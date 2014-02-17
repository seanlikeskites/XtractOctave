# XtractOctave

XtractOctave is a bunch of octave functions which wrap the functions found in LibXtract.

At the moment this has only been tested under Linux.
I'm fairly sure getting it all working under OSX would be pretty simple.
Windows is a completely different matter and will probably take a lot of work.

## Dependencies

You will need LibXtract, which you can find here:
https://github.com/jamiebullock/LibXtract/

For the moment the 'harmonic product spectrum' and 'irregularity' functions don't work properly with that version of LibXtract. 

I have a fork of LibXtract which they work with however.
https://github.com/seanlikeskites/LibXtract/

You will also need the Octave package developer libraries. These are available in the octave-pkg-dev package in ubuntu. 

## Installation

Install the above dependencies first, then (all being well) running make in the XtractOctave directory should build everything for you.

If that worked there should now be a load of .oct file in the directory. If these files are in Octave's search path the functions should all be usable.

## Documentation

Documentation is available in the same way it is for all Octave functions, using the help command in the octave prompt.

## License 

Copyright (C) 2014 Sean Enderby

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
