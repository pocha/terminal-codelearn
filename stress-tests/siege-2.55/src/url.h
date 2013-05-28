/**
 * URL support
 *
 * Copyright (C) 2000, 2001 Jeffrey Fulmer <jdfulmer@armstrong.com>
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
 * --
 */
#ifndef  URL_H
#define  URL_H

int  protocol_length( char *url ); 
int  is_supported( char* url );
int  get_default_port( PROTOCOL p );
PROTOCOL get_protocol( const char *url );
void insert_childid( URL *U, int mypid );
int  process_post_data( char *datap );
URL  build_url( char *url, int defaultport );
URL  add_redirect( char *url, int port );
URL  add_url( char *url );

#endif/*URL_H*/
