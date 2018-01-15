/* 
   
   Copyright (C) 1999, 2000, 2001 Juniper Networks Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.  
   
*/

#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>


#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "runtime.h"
#include "runtime_error_msg.h"
#include "mem_macro.h"
#include "data.h"
#include "exec_macro.h"
#include "code_exec_macro.h"
#include "thread.h"
#include "ip_socket.h"


/****************************************************************
 * ip_socket class : Internet domain connection oriented socket
 ****************************************************************/
/* allocate ip_socket */
void *JD_sa_ip_socket() {
  JD_ip_socket *ip ;
  
  ip = (JD_ip_socket *)calloc(1, sizeof(JD_ip_socket) ) ;
  
  return((void *)ip) ;
  
}

char *JD_disp_ip_socket( char *indent, JD_data *dt, char *eol ) {
  char *ret ;
  JD_ip_socket *ip ;
  int i ;
  ip = (void *)dt->data.obj.ect->table ;
  ret = (char *)calloc(strlen(indent)+200,sizeof(char) ) ;
  sprintf( 
    ret, "%s%s orig_sock=%d new_sock=%d %s", indent, 
    ip->opened?"opened":"closed", ip->orig_sock, ip->new_sock,
    eol
   ) ;
  return ret ;
}

void JD_sc_ip_socket_mf_new() {
  
}

/* deallocate ip_socket */
void JD_sd_ip_socket(JD_ip_socket *ip) {
  if( ip->opened ) {
    close(ip->new_sock) ;
  }
  free(ip) ;
}

void *JD_dup_ip_socket(JD_ip_socket *rd) {
  JD_ip_socket *ip ;
  
  PRINTF( "ip_socket: Can't duplicate socket\n" ) ;
  ip = (JD_ip_socket *)calloc(1, sizeof(JD_ip_socket) ) ;
  
  return(ip) ;
}

void JD_sc_ip_socket_mf_server_bind()
{
  JD_data *dt ;
  int port_num ;
  JD_ip_socket *ip ;
  struct sockaddr_in clnt_adr, serv_adr ;
  int clnt_len ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  ip =  (void *)ACC_DT->data.obj.ect->table ;

  dt = GET_STACK(1) ;
  
  if( IS_SMLINT(dt) ) port_num = SMLINT2INT(dt) ;
  else port_num = dt->data.int_value ;
  
  if( ip->opened ) {
    PRINTF( "ip_socket.server_open_port(): port already opened\n" ) ;
    ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  
  if( (ip->orig_sock = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
    PRINTF( "ip_socket.server_open_port(): socket allocation error\n" ) ;
    ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  
  /* debug */
  PRINTF( "server_bind: port = %d\n", port_num ) ;
  
  memset( &serv_adr, 0, sizeof(serv_adr) ) ;
  serv_adr.sin_family = AF_INET ;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY) ;
  serv_adr.sin_port = htons(port_num) ;
  
  if(
    bind( ip->orig_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr) ) < 0
  )
  {
    PRINTF( "ip_socket.server_open_port(): bind error\n" ) ;
    close(ip->orig_sock) ;
    ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
}

void JD_sc_ip_socket_mf_server_accept()
{
  JD_ip_socket *ip ;
  struct sockaddr_in clnt_adr ;
  int clnt_len ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  ip =  (void *)ACC_DT->data.obj.ect->table ;
  
  if( listen(ip->orig_sock, 5) < 0 ) {
    PRINTF( "ip_socket.server_open_port(): listen error\n" ) ;
    close(ip->orig_sock) ;
    ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  
  clnt_len = sizeof(clnt_adr) ;
  if(
    (ip->new_sock = 
      accept( ip->orig_sock, (struct sockaddr *)&clnt_adr, &clnt_len ) ) < 0 )
  {
    PRINTF( "ip_socket.server_open_port(): accept error\n" ) ;
    close(ip->orig_sock) ;
    ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  ip->opened = 1 ;
  ACC_INT = 1 ;
  ACC_TYPE = JD_D_int ;
}

void JD_sc_ip_socket_mf_client_open_port()
{
  JD_data *dt, *dt1 ;
  int port_num ;
  JD_ip_socket *ip ;
  struct sockaddr_in serv_adr ;
  struct hostent *host ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  ip =  (void *)ACC_DT->data.obj.ect->table ;

  dt = GET_STACK(1) ;
  if( IS_SMLINT(dt) ) port_num = SMLINT2INT(dt) ;
  else port_num = dt->data.int_value ;
  
  dt1 = GET_STACK(2) ;
  
  if( ip->opened ) {
    PRINTF( "ip_socket.client_open_port(): port already opened\n" ) ;
    ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  if( dt1->data.string == NULL ) 
    host = (struct hostent *)gethostbyname( dt1->data.string ) ;
  else {
    PRINTF( 
      "ip_socket.client_open_port(): hostname is null\n",
      dt1->data.string
    ) ;
    ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  
  if( host == NULL ) {
    PRINTF( 
      "ip_socket.client_open_port(): gethostbyname( \"%s\" ) failed\n",
      dt1->data.string
    ) ;
    ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  
  if( (ip->orig_sock = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
    PRINTF( "ip_socket.client_open_port(): socket allocation error\n" ) ;
    ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }

  memset( &serv_adr, 0, sizeof(serv_adr) ) ;
  serv_adr.sin_family = AF_INET ;
  /* serv_adr.sin_addr.s_addr = **host->h_addr_list ;  */
  memcpy(&serv_adr.sin_addr, host->h_addr, host->h_length) ;
  serv_adr.sin_port = htons(port_num) ;
  
  if(
    connect( 
      ip->orig_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)
    ) < 0
  ) {
    PRINTF( "ip_socket.server_open_port(): connect error\n" ) ;
    close(ip->orig_sock) ;
    ACC_INT = 0 ;
    ACC_TYPE = JD_D_int ;
    return ;
  }
  ip->new_sock = ip->orig_sock ; /* copy for rd,wr,close */
  ip->opened = 1 ;
  ACC_INT = 1 ;
  ACC_TYPE = JD_D_int ;
}

void JD_sc_ip_socket_mf_write()
{
  JD_data *dt ;
  JD_ip_socket *ip ;
  char *s ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  ip =  (void *)ACC_DT->data.obj.ect->table ;

  dt = GET_STACK(1) ;
  
  if( !ip->opened ) {
    PRINTF( "ip_socket.write(): port is not opened\n" ) ;
    return ;
  }

  if( !dt->data.string ) {
    PRINTF( "ip_socket.write(): data string is null\n" ) ;
    return ;
  }
    
  
  write(ip->new_sock, dt->data.string, strlen(dt->data.string)) ;
  write(ip->new_sock, "\n", 1) ;
  
}

#define BUFFSIZE 4096
static char buffer[BUFFSIZE+1] ;

void JD_sc_ip_socket_mf_read()
{
  int len ;
  JD_ip_socket *ip ;
  
  if( ACC_DT->data.obj.ect == NULL ) {
    ERROR_EMPTY_OBJECT_ACCESS ;
  }
  ip =  (void *)ACC_DT->data.obj.ect->table ;

  if( !ip->opened ) {
    PRINTF( "ip_socket.read(): port is not opened\n" ) ;
    ACC_STRING = NULL ;
    ACC_TYPE = JD_D_string ;
    return ;
  }

  len = read(ip->new_sock, buffer, BUFFSIZE ) ;
  if( len ) {
    ACC_STRING = (char *)strdup(buffer) ;
  }
  else ACC_STRING = NULL ;
  ACC_TYPE = JD_D_string ;
  
}

