sysconfdir=${prefix}/etc
piddir=/var/run
srcdir=.
top_srcdir=..


CC=arm-linux-gcc
LD=arm-linux-gcc
CFLAGS=-g -O2 -pipe -Wall -Wpointer-arith -Wuninitialized -Wsign-compare -Wformat-security -Wno-pointer-sign -fno-strict-aliasing -D_FORTIFY_SOURCE=2 -ftrapv -fno-builtin-memset -fstack-protector-all -fPIE  
CPPFLAGS=-I. -I.. -I$(srcdir) -I$(srcdir)/.. -I/home/mbin96/build/src/term/openssh/openssh-8.0p1/../openssl-1.0.2t/build_openssl/include -I../zlib-1.2.11/build/include  -D_XOPEN_SOURCE=600 -D_BSD_SOURCE -D_DEFAULT_SOURCE -DHAVE_CONFIG_H
LIBS=-lcrypto -lrt -ldl -lutil -lz  -lcrypt -lresolv
AR=arm-linux-ar
RANLIB=arm-linux-ranlib
INSTALL=/usr/bin/install -c
LDFLAGS=-L. -L/home/mbin96/build/src/term/openssh/openssh-8.0p1/../openssl-1.0.2t/build_openssl/lib -L../zlib-1.2.11/build/lib  -Wl,-z,retpolineplt -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -fstack-protector-all -pie 

OPENBSD=base64.o \
	basename.o \
	bcrypt_pbkdf.o \
	bcrypt_pbkdf.o \
	bindresvport.o \
	blowfish.o \
	daemon.o \
	dirname.o \
	explicit_bzero.o \
	fmt_scaled.o \
	freezero.o \
	getcwd.o \
	getgrouplist.o \
	getopt_long.o \
	getrrsetbyname.o \
	glob.o \
	inet_aton.o \
	inet_ntoa.o \
	inet_ntop.o \
	md5.o \
	mktemp.o \
	pwcache.o \
	readpassphrase.o \
	reallocarray.o \
	realpath.o \
	recallocarray.o \
	rmd160.o \
	rresvport.o \
	setenv.o \
	setproctitle.o \
	sha1.o \
	sha2.o \
	sigact.o \
	strcasestr.o \
	strlcat.o \
	strlcpy.o \
	strmode.o \
	strndup.o \
	strnlen.o \
	strptime.o \
	strsep.o \
	strtoll.o \
	strtonum.o \
	strtoull.o \
	strtoul.o \
	timingsafe_bcmp.o \
	vis.o

COMPAT=	arc4random.o \
	bsd-asprintf.o \
	bsd-closefrom.o \
	bsd-cygwin_util.o \
	bsd-err.o \
	bsd-flock.o \
	bsd-getline.o \
	bsd-getpagesize.o \
	bsd-getpeereid.o \
	bsd-malloc.o \
	bsd-misc.o \
	bsd-nextstep.o \
	bsd-openpty.o \
	bsd-poll.o \
	bsd-setres_id.o \
	bsd-signal.o \
	bsd-snprintf.o \
	bsd-statvfs.o \
	bsd-waitpid.o \
	fake-rfc2553.o \
	getrrsetbyname-ldns.o \
	kludge-fd_set.o \
	openssl-compat.o \
	libressl-api-compat.o \
	xcrypt.o

PORTS=	port-aix.o \
	port-irix.o \
	port-linux.o \
	port-solaris.o \
	port-net.o \
	port-uw.o

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

all: libopenbsd-compat.a

$(COMPAT): ../config.h
$(OPENBSD): ../config.h
$(PORTS): ../config.h

libopenbsd-compat.a:  $(COMPAT) $(OPENBSD) $(PORTS)
	$(AR) rv $@ $(COMPAT) $(OPENBSD) $(PORTS)
	$(RANLIB) $@

clean:
	rm -f *.o *.a core

distclean: clean
	rm -f Makefile *~
