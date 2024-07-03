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
//#include"xml.h"

typedef struct {
  uint16_t xid;      
  uint16_t flags;    
  uint16_t qdcount; 
  uint16_t ancount; 
  uint16_t nscount; 
  uint16_t arcount; 
} dns_header_t;

typedef struct {
  char *name;        
  uint16_t dnstype; 
  uint16_t dnsclass; 
} dns_question_t;



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
  uint8_t name[];
} __attribute__((packed)) dns_record_ns_t;






int main(int argc,char**argv)
{
   int socketfd = socket(AF_INET,SOCK_DGRAM,0);
   struct sockaddr_in address;
   
   address.sin_family=AF_INET;
   address.sin_addr.s_addr = htonl(0xd043dede);
   address.sin_port=htons(53);
   
   dns_header_t header;
   memset(&header,0,sizeof(dns_header_t));
   
   header.xid=htons(0x1234);
   header.flags=htons(0x0100);
   header.qdcount=htons(1);
   header.ancount=htons(2);
   
   
   dns_question_t question;
   question.dnstype=htons(2);
   question.dnsclass = htons(1);
   
   
   char hostname[]="leetcode.com";
   
   
   question.name = calloc(strlen(hostname)+2,sizeof(char));
   
  
  
  memcpy (question.name + 1, hostname, strlen (hostname));
  uint8_t *prev = (uint8_t *) question.name;
  uint8_t count = 0; /* Used to count the bytes in a field */

/* Traverse through the name, looking for the . locations */
for (size_t i = 0; i < strlen (hostname); i++)
  {
    /* A . indicates the end of a field */
    if (hostname[i] == '.')
      {
        /* Copy the length to the byte before this field, then
           update prev to the location of the . */
        *prev = count;
        prev = question.name + i + 1;
        count = 0;
      }
    else
      count++;
  }
*prev = count;



size_t packetlen = sizeof (header) + strlen (hostname) + 2 +
  sizeof (question.dnstype) + sizeof (question.dnsclass);
uint8_t *packet = calloc (packetlen, sizeof (uint8_t));
uint8_t *p = (uint8_t *)packet;

/* Copy the header first */
memcpy (p, &header, sizeof (header));
p += sizeof (header);

/* Copy the question name, QTYPE, and QCLASS fields */
memcpy (p, question.name, strlen (hostname) + 1);
p += strlen (hostname) + 2; /* includes 0 octet for end */
memcpy (p, &question.dnstype, sizeof (question.dnstype));
p += sizeof (question.dnstype);
memcpy (p, &question.dnsclass, sizeof (question.dnsclass));

/* Send the packet to OpenDNS, then request the response */
sendto (socketfd, packet, packetlen, 0, (struct sockaddr *) &address, 
        (socklen_t) sizeof (address));
        
       
socklen_t length = 0;
uint8_t response[512];
memset (&response, 0, 512);

/* Receive the response from OpenDNS into a local buffer */
ssize_t bytes = recvfrom (socketfd, response, 512, 0, (struct sockaddr *) &address, &length);






dns_header_t *response_header = (dns_header_t *)response;
assert ((ntohs (response_header->flags) & 0xf) == 0);



uint8_t *start_of_name = (uint8_t *) (response + sizeof (dns_header_t));
uint8_t total = 0;
uint8_t *field_length = start_of_name;



while (*field_length != 0)
  {
    /* Restore the dot in the name and advance to next length */
 //   printf("%s\n",(char*)field_length);
    total += *field_length + 1;
    *field_length = '.';
    field_length = start_of_name + total;
  
  }
  



dns_record_ns_t *records = (dns_record_ns_t *) (field_length + 5);


 printf("Ans:%d\n",response_header->ancount);
 
for (int i = 0;i<ntohs(response_header->ancount); i++)
{
      
      uint8_t j=0; 
      while((records[i].name[j])!=0||((records[i].name[j])&0xc0!=0xc0)){
      
      
   if((records[i].name[j]&0xc0)==0xc0)
      {
      
        //  printf("Found a pointer\n");
          uint8_t byte = records[i].name[j];
          uint8_t byte2 = records[i].name[j+1];
          uint64_t offset = ((byte & 0x3f) << 8) | byte2;
          offset+=(uint64_t)&response[0];
          offset++;
          
          while(*(char*)offset!=0)
          {
             printf("%c",*(char*)offset);
             offset++;
          
          }
          j=j+2;
         
      }
      
      else
      {
        printf("%c",records[i].name[j]);
        j++;
      
      }
      
      

}
 printf("\n");
}






printf("\n======================\n");
for(int i=0;i<512;i++)
{
   if(response[i].name&0xc0==0xc0)
   {printf("Pointer");}

}
printf("\n======================\n");





}







/*




  while((records[i].name[j])!=0){
      
      
   if((records[i].name[j]&0xc0)==0xc0)
      {
      
        //  printf("Found a pointer\n");
          uint8_t byte = records[i].name[j];
          uint8_t byte2 = records[i].name[j+1];
          uint64_t offset = ((byte & 0x3f) << 8) | byte2;
          offset+=(uint64_t)&response[0];
        //  offset+=sizeof(dns_header_t);
          
         // printf("%" PRIx64 "\n",offset);
         // printf("%" PRIx64 "\n",&response[0]); 
          offset++;
          
          while(*(char*)offset!=0)
          {
             printf("%c",*(char*)offset);
             offset++;
          
          }
          printf(".");
          j=j+2;
         
         }
      
      else
      {
         printf("%c",records[i].name[j]);
        j++;
      
      }























*/













