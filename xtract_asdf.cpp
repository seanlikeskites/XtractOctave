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

DEFUN_DLD (xtract_asdf, args, nargout,
"-*- texinfo -*-\n"
"@deftypefn {Function File} {} xtract_asdf (@var{data})\n"
"Calculate the average square difference function of the signal @var{data}.\n"
"\n"
"A wrapper for LibXtract\'s xtract_asdf function.\n"
"@end deftypefn\n")
{
    // make sure the correct amount of arguments have been passed
    if (args.length() != 1)
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

        // find asdf
        OCTAVE_LOCAL_BUFFER (double, asdf, inputLength);
        xtract_asdf (inputData, inputLength, NULL, asdf);

        // put into an output vector
        RowVector output (inputLength);
        int n = inputLength;
        while (n--)
        {
            output (n) = asdf [n];
        }

        return octave_value (output);
    }
}
