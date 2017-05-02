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


### Installing prerequisites on Mac OS X

We use Vaeql as native extension for HHVM, so we need to build (or rebuild) all HHVM from source.

    brew install llvm
    brew install freetype gettext cmake libtool mcrypt oniguruma  \
             autoconf libelf readline automake md5sha1sum \
             gd icu4c libmemcached pkg-config tbb imagemagick@6 \
             libevent sqlite openssl glog boost lz4 pcre \
             gawk jemalloc ocaml gmp dwarfutils libzip
    git clone --recursive git://github.com/facebook/hhvm.git

    cd hhvm/hphp/runtime/ext
    git clone https://github.com/wowazzz/hhvm_vaeql.git vaeql
    cd vaeql
    mv config.cmake.osx config.cmake
    mv vaeql.patch ../../../../vaeql.patch
    cd ../../../../
    git am vaeql.patch

    cmake . \
    -DCMAKE_CXX_COMPILER=$(brew --prefix llvm)/bin/clang++ \
    -DCMAKE_C_COMPILER=$(brew --prefix llvm)/bin/clang \
    -DCMAKE_ASM_COMPILER=$(brew --prefix llvm)/bin/clang \
    -DCMAKE_C_FLAGS="-I$(brew --prefix readline)/include -L$(brew --prefix readline)/lib" \
    -DCMAKE_CXX_FLAGS="-I$(brew --prefix readline)/include -L$(brew --prefix readline)/lib" \
    -DENABLE_MCROUTER=OFF \
    -DENABLE_EXTENSION_MCROUTER=OFF \
    -DENABLE_EXTENSION_IMAP=OFF \
    -DMYSQL_UNIX_SOCK_ADDR=/tmp/mysql.sock \
    -DLIBEVENT_LIB=$(brew --prefix libevent)/lib/libevent.dylib \
    -DLIBEVENT_INCLUDE_DIR=$(brew --prefix libevent)/include \
    -DICU_INCLUDE_DIR=$(brew --prefix icu4c)/include \
    -DICU_LIBRARY=$(brew --prefix icu4c)/lib/libicuuc.dylib \
    -DICU_I18N_LIBRARY=$(brew --prefix icu4c)/lib/libicui18n.dylib \
    -DICU_DATA_LIBRARY=$(brew --prefix icu4c)/lib/libicudata.dylib \
    -DREADLINE_INCLUDE_DIR=$(brew --prefix readline)/include \
    -DREADLINE_LIBRARY=$(brew --prefix readline)/lib/libreadline.dylib \
    -DBOOST_INCLUDEDIR=$(brew --prefix boost)/include \
    -DBOOST_LIBRARYDIR=$(brew --prefix boost)/lib \
    -DLIBINTL_LIBRARIES=$(brew --prefix gettext)/lib/libintl.dylib \
    -DLIBINTL_INCLUDE_DIR=$(brew --prefix gettext)/include \
    -DLIBDWARF_LIBRARIES=$(brew --prefix dwarfutils)/lib/libdwarf.a \
    -DLIBDWARF_INCLUDE_DIRS=$(brew --prefix dwarfutils)/include \
    -DLIBMAGICKWAND_INCLUDE_DIRS=$(brew --prefix imagemagick@6)/include/ImageMagick-6 \
    -DLIBMAGICKWAND_LIBRARIES=$(brew --prefix imagemagick@6)/lib/libMagickWand-6.Q16.dylib \
    -DLIBMAGICKCORE_LIBRARIES=$(brew --prefix imagemagick@6)/lib/libMagickCore-6.Q16.dylib \
    -DFREETYPE_INCLUDE_DIRS=$(brew --prefix freetype)/include/freetype2 \
    -DFREETYPE_LIBRARIES=$(brew --prefix freetype)/lib/libfreetype.dylib \
    -DLIBMEMCACHED_LIBRARY=$(brew --prefix libmemcached)/lib/libmemcached.dylib \
    -DLIBMEMCACHED_INCLUDE_DIR=$(brew --prefix libmemcached)/include \
    -DLIBELF_LIBRARIES=$(brew --prefix libelf)/lib/libelf.a \
    -DLIBELF_INCLUDE_DIRS=$(brew --prefix libelf)/include/libelf \
    -DLIBGLOG_LIBRARY=$(brew --prefix glog)/lib/libglog.dylib \
    -DLIBGLOG_INCLUDE_DIR=$(brew --prefix glog)/include \
    -DOPENSSL_SSL_LIBRARY=$(brew --prefix openssl)/lib/libssl.dylib \
    -DOPENSSL_INCLUDE_DIR=$(brew --prefix openssl)/include \
    -DOPENSSL_CRYPTO_LIBRARY=$(brew --prefix openssl)/lib/libcrypto.dylib \
    -DCRYPT_LIB=$(brew --prefix openssl)/lib/libcrypto.dylib \
    -DTBB_INSTALL_DIR=$(brew --prefix tbb) \
    -DLIBSQLITE3_INCLUDE_DIR=$(brew --prefix sqlite)/include \
    -DLIBSQLITE3_LIBRARY=$(brew --prefix sqlite)/lib/libsqlite3.0.dylib \
    -DLIBZIP_INCLUDE_DIR_ZIP=$(brew --prefix libzip)/include \
    -DLIBZIP_INCLUDE_DIR_ZIPCONF=$(brew --prefix libzip)/lib/libzip/include \
    -DLIBZIP_LIBRARY=$(brew --prefix libzip)/lib/libzip.dylib \
    -DLZ4_INCLUDE_DIR=$(brew --prefix lz4)/include \
    -DLZ4_LIBRARY=$(brew --prefix lz4)/lib/liblz4.dylib \
    -DPCRE_INCLUDE_DIR=$(brew --prefix pcre)/include \
    -DPCRE_LIBRARY=$(brew --prefix pcre)/lib/libpcre.dylib \
    -DSYSTEM_PCRE_HAS_JIT=1
    make -j4 hhvm


## Compiling

Only for Linux. We don't need to do this on Mac OS X.

    hphpize
    cmake .
    make
    

## Testing

This project is tested entirely using the test suite in Vae Remote.  It
is very easy to add more tasks to that test suite and we likely do not
need one here.


### Testing on Linux

    cd vae_remote
    hhvm -vDynamicExtensions.0=/path/to/extension/vaeql.so -vEval.Jit=true tests/_all.php


### Testing on Mac OS X

    cd vae_remote
    hhvm tests/_all.php


