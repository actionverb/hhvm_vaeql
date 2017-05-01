# VaeQL

HHVM port for PHP Extension that provides accelerated parsing for VaeDB queries.

Build and install this before Vae Remote.


## License

Copyright (c) 2007-2016 Action Verb, LLC.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program, in the file called COPYING-AGPL.
If not, see http://www.gnu.org/licenses/.


## Prerequisites

 - HHVM
 - libantlr3c (MUST be version 3.2)


DO NOT install libantlr3c using Homebrew, as that only supplies 
version 3.4.
On Linux, you also need to compile it from the source.

To compile libantlr3c:

    wget http://www.antlr3.org/download/C/libantlr3c-3.2.tar.gz
    tar -zxvf libantlr3c-3.2.tar.gz
    cd libantlr3c-3.2
    ./configure --enable-64bit
    make
    make install


### Installing prerequisites on Linux

    apt install hhvm hhvm-dev libgoogle-glog-dev libtbb-dev


### Installing prerequisites on Mac

In progress..


## Compiling

    make
    

## Testing

This project is tested entirely using the test suite in Vae Remote.  It
is very easy to add more tasks to that test suite and we likely do not
need one here.

    cd vae_remote
    hhvm -vDynamicExtensions.0=/path/to/extension/vaeql.so -vEval.Jit=true tests/_all.php


