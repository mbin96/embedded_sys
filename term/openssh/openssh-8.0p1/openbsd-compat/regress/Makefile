sysconfdir=${prefix}/etc
piddir=/var/run
srcdir=.
top_srcdir=../..


CC=arm-linux-gcc
LD=arm-linux-gcc
CFLAGS=-g -O2 -pipe -Wall -Wpointer-arith -Wuninitialized -Wsign-compare -Wformat-security -Wno-pointer-sign -fno-strict-aliasing -D_FORTIFY_SOURCE=2 -ftrapv -fno-builtin-memset -fstack-protector-all -fPIE  
CPPFLAGS=-I. -I.. -I$(srcdir) -I$(srcdir)/.. -I/home/mbin96/build/src/term/openssh/openssh-8.0p1/../openssl-1.0.2t/build_openssl/include -I../zlib-1.2.11/build/include  -D_XOPEN_SOURCE=600 -D_BSD_SOURCE -D_DEFAULT_SOURCE -DHAVE_CONFIG_H
EXEEXT=
LIBCOMPAT=../libopenbsd-compat.a
LIBS=-lcrypto -lrt -ldl -lutil -lz  -lcrypt -lresolv
LDFLAGS=-L/home/mbin96/build/src/term/openssh/openssh-8.0p1/../openssl-1.0.2t/build_openssl/lib -L../zlib-1.2.11/build/lib  -Wl,-z,retpolineplt -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -fstack-protector-all -pie  $(LIBCOMPAT)

TESTPROGS=closefromtest$(EXEEXT) snprintftest$(EXEEXT) strduptest$(EXEEXT) \
	strtonumtest$(EXEEXT) opensslvertest$(EXEEXT) utimensattest$(EXEEXT)

all:	t-exec ${OTHERTESTS}

%$(EXEEXT):	%.c $(LIBCOMPAT)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $< $(LIBCOMPAT) $(LIBS)

t-exec:	$(TESTPROGS)
	@echo running compat regress tests
	@for TEST in ""$?; do \
		echo "run test $${TEST}" ... 1>&2; \
		./$${TEST}$(EXEEXT) || exit $$? ; \
	done
	@echo finished compat regress tests

clean:
	rm -f *.o *.a core $(TESTPROGS) valid.out

distclean: clean
	rm -f Makefile *~
