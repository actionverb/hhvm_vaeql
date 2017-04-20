# MAN, I KNOW MAKEFILE IS WEIRD NOW. DEBUG VERSION,ANYWAY..

C = gcc
CFLAGS = -g -O2 -fPIC -Wno-parentheses -Wno-macro-redefined -Wno-pointer-sign -Wno-tautological-compare -Wno-return-type -I/usr/local/include -I. -c
LDFLAGS =  -L/usr/local/lib -g
LIBS =  -lantlr3c

GEN_SRC = VaeQueryLanguageLexer.c VaeQueryLanguageParser.c VaeQueryLanguageTreeParser.c VaeQueryLanguage.tokens
GEN_HEADERS = VaeQueryLanguageLexer.h VaeQueryLanguageParser.h VaeQueryLanguageTreeParser.h
OBJS = VaeQueryLanguageLexer.o VaeQueryLanguageParser.o VaeQueryLanguageTreeParser.o
HEADERS = ${GEN_HEADERS}
	
default: vaeql.so

clean:
	$(RM) vaeql *.o *.so

generate: VaeQueryLanguage.g VaeQueryLanguageTreeParser.g
	java -classpath vendor/antlr-3.2.jar org.antlr.Tool VaeQueryLanguage.g VaeQueryLanguageTreeParser.g

#install: install-vaeql.so

#install-vaeql.so: vaeql.so
#	mkdir -p `php-config --extension-dir`
#	sudo cp vaeql.so `php-config --extension-dir`

ext_vaeql.o: ext_vaeql.cpp
	/usr/bin/c++   -DDEFAULT_CONFIG_DIR=\"/etc/hhvm/\" -DENABLE_FASTCGI=1 -DENABLE_ZEND_COMPAT=1 -DFOLLY_HAVE_CLOCK_GETTIME=1 -DFOLLY_HAVE_FEATURES_H=1 -DFOLLY_HAVE_PTHREAD_ATFORK=1 -DFOLLY_HAVE_PTHREAD_SPINLOCK_T=1 -DFOLLY_HAVE_VLA=1 -DFOLLY_HAVE_WEAK_SYMBOLS=1 -DFOLLY_NO_CONFIG=1 -DHAVE_BOOST1_49 -DHAVE_CURL_MULTI_WAIT -DHAVE_ELF_GETSHDRSTRNDX -DHAVE_FEATURES_H=1 -DHAVE_LIBDL -DHHVM -DHHVM_BUILD_DSO -DHHVM_DYNAMIC_EXTENSION_DIR=\"/usr/lib/x86_64-linux-gnu/hhvm/extensions\" -DHPHP_OSS=1 -DLIBDWARF_CONST_NAME -DMBFL_STATIC -DNDEBUG -DNO_LIB_GFLAGS -DNO_TCMALLOC=1 -DPACKAGE=hhvm -DPACKAGE_VERSION=Release -DPHP_MYSQL_UNIX_SOCK_ADDR=\"/dev/null\" -DRELEASE=1 -DTHRIFT_MUTEX_EMULATE_PTHREAD_TIMEDLOCK -DUSE_CMAKE -DUSE_EDITLINE -DUSE_JEMALLOC=1 -D_GNU_SOURCE -D_PTHREADS=1 -D_REENTRANT=1 -D__STDC_FORMAT_MACROS -Dvaeql_EXPORTS -I/usr/include/x86_64-linux-gnu -I/usr/include/mysql -I/usr/include/libxml2 -I/usr/include/double-conversion -I/usr/lib/x86_64-linux-gnu/libzip/include -I/usr/include/libdwarf -I/usr/include/hphp -I/usr/include/hphp/third-party/fastlz -I/usr/include/hphp/third-party/timelib -I/usr/include/hphp/third-party/libafdt/src -I/usr/include/hphp/third-party/libmbfl -I/usr/include/hphp/third-party/libmbfl/mbfl -I/usr/include/hphp/third-party/libmbfl/filters -I/usr/include/hphp/third-party/proxygen/src -I/usr/include/hphp/third-party/folly/src -I/usr/include/hphp/third-party/thrift/src -I/usr/include/hphp/third-party/wangle/src -I/usr/include/hphp/third-party  -Wall -std=gnu++11 -ffunction-sections -fdata-sections -fno-gcse -fno-omit-frame-pointer -Woverloaded-virtual -Wno-deprecated -Wno-strict-aliasing -Wno-write-strings -Wno-invalid-offsetof -fno-operator-names -Wno-error=array-bounds -Wno-error=switch -Werror=format-security -Wno-unused-result -Wno-sign-compare -Wparentheses -Wno-attributes -Wno-maybe-uninitialized -Wcomment -Wno-unused-local-typedefs -fno-canonical-system-headers -Wno-deprecated-declarations -Wno-unused-function -Wvla  -ftrack-macro-expansion=0 -fno-builtin-memcmp -fno-delete-null-pointer-checks -Wno-bool-compare -DFOLLY_HAVE_MALLOC_H  -O2 -g -DNDEBUG -fPIC   -o ext_vaeql.cpp.o -c ext_vaeql.cpp
	
vaeql.o: vaeql.c
	${C} ${CFLAGS} vaeql.c
	
vaeql.so: ${OBJS} ext_vaeql.o
#	${C} ${LDFLAGS} -shared -fPIC -Wl,-undefined,dynamic_lookup php_vaeql.o ${OBJS} ${LIBS} -o vaeql.so
	/usr/bin/c++  -fPIC  -Wall -std=gnu++11 -ffunction-sections -fdata-sections -fno-gcse -fno-omit-frame-pointer -Woverloaded-virtual -Wno-deprecated -Wno-strict-aliasing -Wno-write-strings -Wno-invalid-offsetof -fno-operator-names -Wno-error=array-bounds -Wno-error=switch -Werror=format-security -Wno-unused-result -Wno-sign-compare -Wno-attributes -Wno-maybe-uninitialized -Wno-unused-local-typedefs -fno-canonical-system-headers -Wno-deprecated-declarations -Wno-unused-function -Wvla  -ftrack-macro-expansion=0 -fno-builtin-memcmp -fno-delete-null-pointer-checks -Wno-bool-compare -DFOLLY_HAVE_MALLOC_H  -O2 -g -DNDEBUG  -shared -Wl,-soname,vaeql.so ${OBJS} ${LIBS} -o vaeql.so ext_vaeql.cpp.o 
	objcopy --add-section ext.4251a729a073=ext_vaeql.php vaeql.so
	
VaeQueryLanguageLexer.o: ${HEADERS} VaeQueryLanguageLexer.c
	${C} ${CFLAGS} VaeQueryLanguageLexer.c
	
VaeQueryLanguageParser.o: ${HEADERS} VaeQueryLanguageParser.c
	${C} ${CFLAGS} VaeQueryLanguageParser.c
	
VaeQueryLanguageTreeParser.o: ${HEADERS} VaeQueryLanguageTreeParser.c
	${C} ${CFLAGS} VaeQueryLanguageTreeParser.c
