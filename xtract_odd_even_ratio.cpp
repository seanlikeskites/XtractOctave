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

DEFUN_DLD (xtract_odd_even_ratio, args, nargout,
"-*- texinfo -*-\n"
"@deftypefn {Function File} {} xtract_odd_even_ratio (@var{data}, @var{fs}, @var{f0})\n"
"@deftypefnx {Function File} {} xtract_odd_even_ratio (@var{data}, @var{fs}, @var{f0}, @var{threshold})\n"
"Calculate the ratio of odd and even harmonics in the signal @var{data} with sample rate @var{fs} and fundamental frequency @var{f0}.\n"
"\n"
"A wrapper for LibXtract\'s xtract_odd_even_ratio function.\n"
"\n"
"@var{threshold} is the threshold used when finding the harmonic partials. It takes a value between 0 and 1 inclusive. If no value is given this will be set to 0.2.\n"
"@end deftypefn\n")
{
    // make sure the correct amount of arguments have been passed
    if (! ((args.length() > 2) && (args.length() < 5)))
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

        // get the sample rate
        double sampleRate = args (1).double_value();
        double sampleRateByN = sampleRate / paddedLength;

        double argumentArray [4] = {sampleRateByN, XTRACT_MAGNITUDE_SPECTRUM, 0, 0};

        // assign memory for the output of the xtract_spectrum function
        OCTAVE_LOCAL_BUFFER (double, spectrum, paddedLength);

        // initialise and run the fft
        xtract_init_fft (paddedLength, XTRACT_SPECTRUM);
        xtract_spectrum (paddedInput, paddedLength, argumentArray, spectrum);

        // assign memory for the peak finding algorithms
        OCTAVE_LOCAL_BUFFER (double, peaks, paddedLength);

        // find spectral peaks
        argumentArray [1] = 10;
        xtract_peak_spectrum (spectrum, paddedLength / 2, argumentArray, peaks);
        
        // get f0
        double f0 = args (2).double_value();

        // get threshold
        double threshold = 0;
        if (args.length() == 4)
        {
            threshold = args (3).double_value();

            // make sure threshold is within the correct range
            if (! ((threshold >= 0) && (threshold <=1)))
            {
                octave_stdout << "THRESHOLD must be between 0 and 1.\n\n";
                print_usage();
                return octave_value_list();
            }
        }
        else
        {
            threshold = 0.2;
        }

        // find harmonics
        OCTAVE_LOCAL_BUFFER (double, harmonics, paddedLength);
        argumentArray [0] = f0;
        argumentArray [1] = threshold;
        xtract_harmonic_spectrum (peaks, paddedLength, argumentArray, harmonics);

        // find the ratio of odd to even harmonics
        double oddEvenRatio = 0;
        xtract_odd_even_ratio (harmonics, paddedLength / 2, NULL, &oddEvenRatio);

        return octave_value (oddEvenRatio);
    }
}
