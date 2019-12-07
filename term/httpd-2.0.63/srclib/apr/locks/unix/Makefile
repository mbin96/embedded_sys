srcdir = .


TARGETS = \
	thread_mutex.lo \
	thread_rwlock.lo \
	thread_cond.lo \
	proc_mutex.lo \
	global_mutex.lo

# bring in rules.mk for standard functionality
include /home/mbin96/build/src/term/httpd-2.0.63/srclib/apr/build/apr_rules.mk

INCDIR=../../include
INCDIR2=$(INCDIR)/arch
INCDIR3=$(INCDIR)/arch/unix
INCLUDES=-I$(INCDIR) -I$(INCDIR2) -I$(INCDIR3)

# DO NOT REMOVE
