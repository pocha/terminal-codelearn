##
## src/Makefile.am
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

bin_PROGRAMS       =   siege

INCLUDES           =   $(SSL_INCLUDE)

CFLAGS             =   $(PTHREAD_CFLAGS)

LDFLAGS            =   $(SSL_LDFLAGS) $(PTHREAD_LDFLAGS)  

LIBS               =   $(SSL_LIBS) 

siege_LDADD        =   $(top_srcdir)/lib/joedog/libjoedog.la

siege_DEPENDENCIES =   $(top_srcdir)/lib/joedog/libjoedog.la 

siege_SOURCES      =   \
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

AUTOMAKE_OPTIONS   =   foreign no-dependencies                   
 
