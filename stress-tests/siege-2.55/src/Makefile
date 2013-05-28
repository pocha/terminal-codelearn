# Makefile.in generated automatically by automake 1.5 from Makefile.am.

# Copyright 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001
# Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



SHELL = /bin/sh

srcdir = .
top_srcdir = ..

prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include
pkgdatadir = $(datadir)/siege
pkglibdir = $(libdir)/siege
pkgincludedir = $(includedir)/siege
top_builddir = ..

ACLOCAL = ${SHELL} /home/pocha/rails/terminal-codelearn/stress-tests/siege-2.55/utils/missing --run aclocal
AUTOCONF = ${SHELL} /home/pocha/rails/terminal-codelearn/stress-tests/siege-2.55/utils/missing --run autoconf
AUTOMAKE = ${SHELL} /home/pocha/rails/terminal-codelearn/stress-tests/siege-2.55/utils/missing --run automake
AUTOHEADER = ${SHELL} /home/pocha/rails/terminal-codelearn/stress-tests/siege-2.55/utils/missing --run autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
INSTALL_HEADER = $(INSTALL_DATA)
transform = s,x,x,
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = 
host_triplet = i686-pc-linux-gnu
AMTAR = ${SHELL} /home/pocha/rails/terminal-codelearn/stress-tests/siege-2.55/utils/missing --run tar
AS = @AS@
AWK = mawk
CC = gcc
CC_R = 
DATE = March-08-2013
DEPDIR = .deps
DLLTOOL = @DLLTOOL@
ECHO = /bin/echo
EXEEXT = 
INSTALL_STRIP_PROGRAM = ${SHELL} $(install_sh) -c -s
LIBADD_DL = @LIBADD_DL@
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LN_S = ln -s
OBJDUMP = @OBJDUMP@
OBJEXT = o
PACKAGE = siege
PLATFORM = pc-i686-linux-gnu
PTHREAD_CFLAGS = -D_REENTRANT -D_POSIX_PTHREAD_SEMANTICS
PTHREAD_LDFLAGS = -lpthread
RANLIB = ranlib
SSL_INCLUDE = 
SSL_LDFLAGS = 
SSL_LIBS = 
STRIP = strip
VERSION = 2.55
am__include = include
am__quote = 
install_sh = /home/pocha/rails/terminal-codelearn/stress-tests/siege-2.55/utils/install-sh

bin_PROGRAMS = siege

INCLUDES = $(SSL_INCLUDE)

CFLAGS = $(PTHREAD_CFLAGS)

LDFLAGS = $(SSL_LDFLAGS) $(PTHREAD_LDFLAGS)  

LIBS = $(SSL_LIBS) 

siege_LDADD = $(top_srcdir)/lib/joedog/libjoedog.la

siege_DEPENDENCIES = $(top_srcdir)/lib/joedog/libjoedog.la 

siege_SOURCES = \
base64.c   base64.h    \
client.c   client.h    \
cookie.c   cookie.h    \
cfg.c      cfg.h       \
getopt.c   getopt.h    \
version.c  getopt1.c   \
handler.c  handler.h   \
http.c     http.h      \
init.c     init.h      \
load.c     load.h      \
log.c      log.h       \
main.c     setup.h     \
sock.c     sock.h      \
ssl.c      ssl.h       \
timer.c    timer.h     \
url.c      url.h       \
util.c     util.h   


AUTOMAKE_OPTIONS = foreign no-dependencies                   
subdir = src
mkinstalldirs = $(SHELL) $(top_srcdir)/utils/mkinstalldirs
CONFIG_HEADER = $(top_builddir)/include/config.h
CONFIG_CLEAN_FILES =
bin_PROGRAMS = siege$(EXEEXT)
PROGRAMS = $(bin_PROGRAMS)

am_siege_OBJECTS = base64.$(OBJEXT) client.$(OBJEXT) cookie.$(OBJEXT) \
	cfg.$(OBJEXT) getopt.$(OBJEXT) version.$(OBJEXT) \
	getopt1.$(OBJEXT) handler.$(OBJEXT) http.$(OBJEXT) \
	init.$(OBJEXT) load.$(OBJEXT) log.$(OBJEXT) main.$(OBJEXT) \
	sock.$(OBJEXT) ssl.$(OBJEXT) timer.$(OBJEXT) url.$(OBJEXT) \
	util.$(OBJEXT)
siege_OBJECTS = $(am_siege_OBJECTS)
siege_LDFLAGS =

DEFS = -DHAVE_CONFIG_H
DEFAULT_INCLUDES =  -I. -I$(srcdir) -I$(top_builddir)/include
CPPFLAGS = 
depcomp =
COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
LTCOMPILE = $(LIBTOOL) --mode=compile $(CC) $(DEFS) $(DEFAULT_INCLUDES) \
	$(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(LIBTOOL) --mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) \
	$(AM_LDFLAGS) $(LDFLAGS) -o $@
DIST_SOURCES = $(siege_SOURCES)
DIST_COMMON = Makefile.am Makefile.in
SOURCES = $(siege_SOURCES)

all: all-am

.SUFFIXES:
.SUFFIXES: .c .lo .o .obj

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

distclean-libtool:
	-rm -f libtool
$(srcdir)/Makefile.in:  Makefile.am  $(top_srcdir)/configure.in $(ACLOCAL_M4)
	cd $(top_srcdir) && \
	  $(AUTOMAKE) --foreign  src/Makefile
Makefile:  $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) && \
	  CONFIG_HEADERS= CONFIG_LINKS= \
	  CONFIG_FILES=$(subdir)/$@ $(SHELL) ./config.status
install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  p1=`echo $$p|sed 's/$(EXEEXT)$$//'`; \
	  if test -f $$p \
	     || test -f $$p1 \
	  ; then \
	    f=`echo $$p1|sed '$(transform);s/$$/$(EXEEXT)/'`; \
	   echo " $(INSTALL_PROGRAM_ENV) $(LIBTOOL) --mode=install $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/$$f"; \
	   $(INSTALL_PROGRAM_ENV) $(LIBTOOL) --mode=install $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/$$f; \
	  else :; fi; \
	done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  f=`echo $$p|sed 's/$(EXEEXT)$$//;$(transform);s/$$/$(EXEEXT)/'`; \
	  echo " rm -f $(DESTDIR)$(bindir)/$$f"; \
	  rm -f $(DESTDIR)$(bindir)/$$f; \
	done

clean-binPROGRAMS:
	-test -z "$(bin_PROGRAMS)" || rm -f $(bin_PROGRAMS)
siege$(EXEEXT): $(siege_OBJECTS) $(siege_DEPENDENCIES) 
	@rm -f siege$(EXEEXT)
	$(LINK) $(siege_LDFLAGS) $(siege_OBJECTS) $(siege_LDADD) $(LIBS)

mostlyclean-compile:
	-rm -f *.$(OBJEXT) core *.core

distclean-compile:
	-rm -f *.tab.c

.c.o:
	$(COMPILE) -c `test -f $< || echo '$(srcdir)/'`$<

.c.obj:
	$(COMPILE) -c `cygpath -w $<`

.c.lo:
	$(LTCOMPILE) -c -o $@ `test -f $< || echo '$(srcdir)/'`$<
uninstall-info-am:

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP) $(TAGS_FILES)
	list='$(SOURCES) $(HEADERS) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	mkid -fID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) \
		$(TAGS_FILES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || etags $(ETAGS_ARGS) $$tags  $$unique $(LISP)

GTAGS:
	here=`CDPATH=: && cd $(top_builddir) && pwd` \
	  && cd $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) $$here

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH

DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)

top_distdir = ..
distdir = $(top_distdir)/$(PACKAGE)-$(VERSION)

distdir: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  if test -f $$file; then d=.; else d=$(srcdir); fi; \
	  dir=`echo "$$file" | sed -e 's,/[^/]*$$,,'`; \
	  if test "$$dir" != "$$file" && test "$$dir" != "."; then \
	    $(mkinstalldirs) "$(distdir)/$$dir"; \
	  fi; \
	  if test -d $$d/$$file; then \
	    cp -pR $$d/$$file $(distdir) \
	    || exit 1; \
	  else \
	    test -f $(distdir)/$$file \
	    || cp -p $$d/$$file $(distdir)/$$file \
	    || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-am
all-am: Makefile $(PROGRAMS)

installdirs:
	$(mkinstalldirs) $(DESTDIR)$(bindir)

install: install-am
install-exec: install-exec-am
install-data: install-data-am
uninstall: uninstall-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	  `test -z '$(STRIP)' || \
	    echo "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'"` install
mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES) stamp-h stamp-h[0-9]*

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-am

clean-am: clean-binPROGRAMS clean-generic clean-libtool mostlyclean-am

distclean: distclean-am

distclean-am: clean-am distclean-compile distclean-generic \
	distclean-libtool distclean-tags

dvi: dvi-am

dvi-am:

info: info-am

info-am:

install-data-am:

install-exec-am: install-binPROGRAMS

install-info: install-info-am

install-man:

installcheck-am:

maintainer-clean: maintainer-clean-am

maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-compile mostlyclean-generic \
	mostlyclean-libtool

uninstall-am: uninstall-binPROGRAMS uninstall-info-am

.PHONY: GTAGS all all-am check check-am clean clean-binPROGRAMS \
	clean-generic clean-libtool distclean distclean-compile \
	distclean-generic distclean-libtool distclean-tags distdir dvi \
	dvi-am info info-am install install-am install-binPROGRAMS \
	install-data install-data-am install-exec install-exec-am \
	install-info install-info-am install-man install-strip \
	installcheck installcheck-am installdirs maintainer-clean \
	maintainer-clean-generic mostlyclean mostlyclean-compile \
	mostlyclean-generic mostlyclean-libtool tags uninstall \
	uninstall-am uninstall-binPROGRAMS uninstall-info-am

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
