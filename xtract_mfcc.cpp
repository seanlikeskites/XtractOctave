/*
 * Copyright (C) 2014 Sean Enderby
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#include <octave/oct.h>
#include <xtract/libxtract.h>

DEFUN_DLD (xtract_mfcc, args, nargout,
"-*- texinfo -*-\n"
"@deftypefn {Function File} {} xtract_mfcc (@var{data}, @var{fs})\n"
"Calculate the mfccs of the signal @var{data}, with sample rate @var{fs}.\n"
"\n"
"A wrapper for LibXtract\'s xtract_mfcc function.\n"
"@end deftypefn\n")
{
    // make sure the correct amount of arguments have been passed
    if (args.length() != 2)
    {
        print_usage();
        return octave_value_list();
    }
    else
    {
        // get the input data
        RowVector input = args (0).row_vector_value();
        int inputLength = input.length();

        int paddedLength = pow (2, ceil (log2 (inputLength)));

        // zero pad the input so it is a power of 2 in length
        OCTAVE_LOCAL_BUFFER (double, paddedInput, paddedLength);
        for (int i = 0; i < paddedLength; ++i)
        {
            if (i < inputLength)
            {
                paddedInput [i] = input (i);
            }
            else
            {
                paddedInput [i] = 0;
            }
        }

        // get sample rate
        double fs = args (1).double_value();
        double sampleRateByN = fs / paddedLength;

        double argumentArray [4] = {sampleRateByN, XTRACT_MAGNITUDE_SPECTRUM, 0, 0};

        // assign memory for the output of the xtract_spectrum function
        OCTAVE_LOCAL_BUFFER (double, spectrum, paddedLength);

        // initialise and run the fft
        xtract_init_fft (paddedLength, XTRACT_SPECTRUM);
        xtract_spectrum (paddedInput, paddedLength, argumentArray, spectrum);

        // set up mfcc stuff
        xtract_mel_filter melFilters;
        melFilters.n_filters = 13;
        OCTAVE_LOCAL_BUFFER (double*, filterArrays, 13);
        melFilters.filters = filterArrays;
        for(int n = 0; n < 13; ++n)
        {
            melFilters.filters [n] = new double [paddedLength];
        }
        
        // find mfccs
        OCTAVE_LOCAL_BUFFER (double, mfccs, 13);
        xtract_init_mfcc(paddedLength / 2, fs / 2, XTRACT_EQUAL_GAIN, 20, 20000, melFilters.n_filters, melFilters.filters);
        xtract_mfcc(spectrum, paddedLength / 2, &melFilters, mfccs);

        // free memory
        for(int n = 0; n < 13; ++n)
        {
            delete[] melFilters.filters [n];
        }

        // put into an output vector
        RowVector output (13);
        int n = 13;
        while (n--)
        {
            output (n) = mfccs [n];
        }

        return octave_value (output);
    }
}
