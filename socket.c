/* $Id: socket.c 1.1 1995/01/01 07:11:14 cthuang Exp $
 *
 * This module has been modified by Radim Kolar for OS/2 emx
 */

/***********************************************************************
  module:       socket.c
  program:      popclient
  SCCS ID:      @(#)socket.c    1.5  4/1/94
  programmer:   Virginia Tech Computing Center
  compiler:     DEC RISC C compiler (Ultrix 4.1)
  environment:  DEC Ultrix 4.3 
  description:  UNIX sockets code.
 ***********************************************************************/
 
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int Socket(const char *host, int clientPort)
{
    int sock;
    unsigned long inaddr;
    struct sockaddr_in ad;
    struct hostent *hp;
    
    memset(&ad, 0, sizeof(ad));// 置0
    ad.sin_family = AF_INET; //协议类型->IPv4
    inaddr = inet_addr(host);// 将点分十进制的IP地址转化为网络字节序的IP地址（类型 unsigned long）
    if (inaddr != INADDR_NONE)
        memcpy(&ad.sin_addr, &inaddr, sizeof(inaddr));//采用网络字节序的IP地址
    else
    {
        hp = gethostbyname(host);//给定主机名或者域名 获取IP地址 
        if (hp == NULL)
            return -1;
        memcpy(&ad.sin_addr, hp->h_addr, hp->h_length);
    }
    ad.sin_port = htons(clientPort);//将clientPort转换为网络字节序（大端模式---整数的高位字节存放在内存的低地址处）
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
  /*
  
    socket函数对应于普通文件的打开操作。普通文件的打开操作返回一个文件描述字，
    而socket()用于创建一个socket描述符（socket descriptor），它唯一标识一个socket。
    这个socket描述字跟文件描述字一样，后续的操作都有用到它，把它作为参数，通过它来进行一些读写操作。
  */
    if (sock < 0)
        return sock;
    if (connect(sock, (struct sockaddr *)&ad, sizeof(ad)) < 0)//客户端通过调用connect函数来建立与TCP服务器的连接。
        return -1;
    return sock;
}

