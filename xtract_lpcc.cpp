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

DEFUN_DLD (xtract_lpcc, args, nargout,
"-*- texinfo -*-\n"
"@deftypefn {Function File} {} xtract_lpcc (@var{data})\n"
"@deftypefnx {Function File} {} xtract_lpcc (@var{data}, @var{order})\n"
"Calculate the linear predictive coding cepstral coefficients of the signal @var{data}.\n"
"\n"
"A wrapper for LibXtract\'s xtract_lpcc function.\n"
"\n"
"@var{order} is an optional argument to chose the length of the resultant array of coefficients. It should be approximatly equal to (1.5 * (N - 1)), where N in the length of the input signal. If no value is given it will be set as close to this value as possible.\n"
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
        const double* inputData = input.data();

        // autocorrelate the input
        OCTAVE_LOCAL_BUFFER (double, autocorrelation, inputLength);
        xtract_autocorrelation (inputData, inputLength, NULL, autocorrelation);

        // find lpc
        int numCoefficients = inputLength - 1;
        OCTAVE_LOCAL_BUFFER (double, lpc, 2 * numCoefficients);
        xtract_lpc (autocorrelation, inputLength, NULL, lpc);

        // get order
        double order = 0;
        if (args.length() == 2)
        {
            order = round (args (1).double_value());
        }
        else
        {
            order = round (1.5 * numCoefficients);
        }
        
        // find lpcc
        OCTAVE_LOCAL_BUFFER (double, lpcc, order);
        xtract_lpcc (lpc + numCoefficients, numCoefficients, &order, lpcc);

        // put into an output vector
        RowVector output (order);
        int n = order;
        while (n--)
        {
            output (n) = lpcc [n];
        }

        return octave_value (output);
    }
}
