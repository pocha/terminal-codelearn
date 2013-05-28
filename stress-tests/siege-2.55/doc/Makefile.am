##
## doc/Makefile.am
##
## Copyright (C) 2000, 2001, 2002 by
## Jeffrey Fulmer - <jdfulmer@armstrong.com>
## This file is distributed as part of Siege
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##

AUTOMAKE_OPTIONS  =    foreign no-dependencies 

man_MANS          =    \
siege.1                \
siege.config.1         \
layingsiege.1          \
urls_txt.1             \
bombardment.1          \
siege2csv.1

EXTRA_DIST        =      $(man_MANS) urls.txt siegerc.in

DISTCLEANFILES    =      siegerc

SIEGERC           =      $(HOME)/.siegerc

URLSTXT           =      $(sysconfdir)/urls.txt

install-exec-hook: 
	@if test -f $(SIEGERC); then \
	  if cmp -s $(srcdir)/siegerc $(SIEGERC); then echo ""; \
	    else \
	    echo ' $(INSTALL_DATA) $(srcdir)/siegerc $(SIEGERC).new'; \
	    $(INSTALL_DATA) $(srcdir)/siegerc $(SIEGERC).new; \
	    echo "#####################################################"; \
	    echo "WARNING: File $(SIEGERC) already exists."; \
	    echo "         A new resource file has been installed as"; \
	    echo "         $(SIEGERC).new. You may want to"; \
	    echo "         consider using the newer version in order to"; \
	    echo "         take advantage of any new features."; \
	    echo "#####################################################"; \
	  fi; \
	else \
	  $(INSTALL_DATA) $(srcdir)/siegerc $(SIEGERC); \
	fi 
	@if test -f $(URLSTXT); then \
	  if cmp -s $(srcdir)/siegerc $(URLSTXT); then echo ""; \
	    else \
	    echo "WARNING: File $(URLSTXT) already exists."; \
	    echo "         It was NOT replaced with this installation."; \
	  fi; \
	else \
	  $(mkinstalldirs) $(sysconfdir); \
	  $(INSTALL_DATA) $(srcdir)/urls.txt $(URLSTXT); \
	fi 


uninstall:
	rm -f $(SIEGERC)

