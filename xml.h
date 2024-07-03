#ifndef __XML__
#define __XML__

#include"mxml.h"
#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<inttypes.h>
#include<assert.h>
#include<stdlib.h>



typedef struct {
  uint16_t compression;
  uint16_t type;
  uint16_t class;
  uint32_t ttl;
  uint16_t length;
  struct in_addr addr;
} __attribute__((packed)) dns_record_a_t;


typedef struct {
  uint16_t compression;
  uint16_t type;
  uint16_t class;
  uint32_t ttl;
  uint16_t length;
  char name[];
} __attribute__((packed)) dns_record_txt_t;





void parseArecord(dns_record_a_t* records,int i);
void parseTXTrecord(dns_record_txt_t* records,int i);
void saveXML();







#endif