/**
 * Cookie support
 *
 * Copyright (C) 2001-2002 Jeffrey Fulmer <jdfulmer@armstrong.com>
 * This file is part of Siege
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#ifndef COOKIES_H
#define COOKIES_H

#ifdef  HAVE_CONFIG_H
# include <config.h>
#endif/*HAVE_CONFIG_H*/

#include <stdlib.h>
#include <joedog/joedog.h>

#define MAX_COOKIE_SIZE 2048   

int  add_cookie( int id, char *host, char *value );
int  delete_cookie( int id, char *name ); 
int  check_cookie( char *domain, char *value );
char *get_cookie( int id, char *domain, char *cookie );

#endif/*COOKIES_H*/

