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

DEFUN_DLD (xtract_flatness, args, nargout,
"-*- texinfo -*-\n"
"@deftypefn {Function File} {} xtract_flatness (@var{data})\n"
"@deftypefnx {Function File} {} xtract_flatness (@var{data}, @var{db})\n"
"Calculate the spectral flatness of the signal @var{data}.\n"
"\n"
"A wrapper for LibXtract\'s xtract_flatness function.\n"
"For now only short lengths of input data (<= 128) should be used. I will endeavour to fix this at some point.\n"
"\n"
"@var{db} is an optional boolean argument to select whether the output is given in decibels or not."
"@end deftypefn\n")
{
    // make sure the correct amount of arguments have been passed
    if (! ((args.length() > 0) && (args.length() < 3)))
    {
        print_usage();
        return octave_value_list();
    }
    else
    {
        // get the input data
        RowVector input = args (0).row_vector_value();
        int inputLength = input.length();

        // warn about too long an input vector
        if (inputLength > 128)
        {
            warning ("This function has serious issue with long input vectors. Try to keep your input vector length at or below 128. If not you are likely to get exceedingly inaccurate values.");
        }


        int paddedLength = pow (2, ceil (log2 (inputLength)));

        // zero pad the input so it is a flatness of 2 in length
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

        double argumentArray [4] = {0, XTRACT_MAGNITUDE_SPECTRUM, 0, 0};

        // assign memory for the output of the xtract_spectrum function
        OCTAVE_LOCAL_BUFFER (double, spectrum, paddedLength);

        // initialise and run the fft
        xtract_init_fft (paddedLength, XTRACT_SPECTRUM);
        xtract_spectrum (paddedInput, paddedLength, argumentArray, spectrum);

        // find the spectral flatness
        double spectralFlatness = 0;
        xtract_flatness (spectrum, paddedLength / 2, NULL, &spectralFlatness);

        // return dB or not
        double db = 0;
        if (args.length() == 2)
        {
            db = args (1).double_value();
        }
        else
        {
            db = false;
        }

        if (db)
        {
            if (spectralFlatness <= 0)
            {
                spectralFlatness = 2e-42;
            }

            spectralFlatness = 10 * log10 (spectralFlatness);
        }

        return octave_value (spectralFlatness);
    }
}
