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

DEFUN_DLD (xtract_f0, args, nargout,
"-*- texinfo -*-\n"
"@deftypefn {Function File} {} xtract_f0 (@var{data}, @var{fs})\n"
"Estimate the fundamental frequency of the signal @var{data} with sample rate @var{fs}.\n"
"\n"
"A wrapper for LibXtract\'s xtract_failsafe_f0 function.\n"
"It will try and use xtract_f0 first.\n "
"If that fails it will return the frequency of the lowest partial in the spectrum.\n"
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
        const double* inputData = input.data();

        // get the sample rate
        double sampleRate = args (1).double_value();

        // find f0
        double f0 = 0;
        int firstExtractionResult = xtract_f0(inputData, inputLength, &sampleRate, &f0);

        // if xtract_f0 fails we find the lowest spectral peak (a la xtract_failsafe_f0)
        if (firstExtractionResult == XTRACT_NO_RESULT)
        {
            // zero pad the input so it is a power of 2 in length
            int paddedLength = pow (2, ceil (log2 (inputLength)));
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

            double sampleRateByN = sampleRate / paddedLength;
            double argumentArray [4] = {sampleRateByN, XTRACT_MAGNITUDE_SPECTRUM, 0, 0};

            // assign memory for the output of the xtract_spectrum function
            OCTAVE_LOCAL_BUFFER (double, spectrum, paddedLength);

            // initialise and run the fft
            xtract_init_fft (paddedLength, XTRACT_SPECTRUM);
            xtract_spectrum (paddedInput, paddedLength, argumentArray, spectrum);

            // find spectral peaks
            OCTAVE_LOCAL_BUFFER (double, peaks, paddedLength);
            argumentArray [1] = 10;
            xtract_peak_spectrum (spectrum, paddedLength / 2, argumentArray, peaks);
            
            // find lowest frequency peak
            argumentArray [0] = 0;
            xtract_lowest_value (peaks + (paddedLength / 2), paddedLength / 2, argumentArray, &f0);
        }

        return octave_value (f0);
    }
}
