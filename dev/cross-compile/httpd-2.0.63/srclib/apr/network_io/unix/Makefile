srcdir = .


TARGETS = \
	sendrecv.lo \
	sockets.lo \
	sockopt.lo \
	sockaddr.lo \
	inet_ntop.lo \
	inet_pton.lo

# bring in rules.mk for standard functionality
include /home/mbin96/build/src/term/httpd-2.0.63/srclib/apr/build/apr_rules.mk

INCDIR=../../include
OSDIR=$(INCDIR)/arch/unix
DEFOSDIR=$(INCDIR)/arch/unix
INCLUDES=-I$(INCDIR) -I$(OSDIR) -I$(DEFOSDIR)

# DO NOT REMOVE
