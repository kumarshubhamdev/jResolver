#include"xml.h"
#include<stdio.h>
#include<stdlib.h>

#include <fcntl.h>
#include <unistd.h>


 mxml_node_t *xml;
   mxml_node_t *data;
   mxml_node_t *node;
   mxml_node_t *group;
   void parseArecord(dns_record_a_t* records,int i){
  
   
   
   xml=mxmlNewXML("1.0");
   
   
   data = mxmlNewElement(xml, "A_RECORD");

  node = mxmlNewElement(data, "Type");
  
  int x =ntohs(records[i].type);
  int length = snprintf( NULL, 0, "%d", x );
  char* str = malloc( length + 1 );
  snprintf( str, length + 1, "%d", x );
  
  
  mxmlNewText(node, false, str);
  
  node = mxmlNewElement(data, "Type");
  mxmlNewText(node, false, str);
  
  node = mxmlNewElement(data, "Class");
  mxmlNewText(node, false, "192.168.1.1");
  
  
  
  
  
  str = malloc( length + 1 );
  str=inet_ntoa (records[i].addr);
  //snprintf( str, length + 1, "%d", x );
  
  node = mxmlNewElement(data, "IPV4");
  mxmlNewText(node, false,str);

  

  
}




void parseTXTrecord(dns_record_txt_t* records,int i){


   
   
xml=mxmlNewXML("1.0");
char *e =malloc(1); 
e[0]=(char)(i+48);
data = mxmlNewElement(xml,"A_RECORD");
  

node = mxmlNewElement(data, "TXT");
mxmlNewText(node, false,records[i].name);

//mxml_options_t *options = mxmlOptionsNew();
  
//mxmlSaveFilename(xml, /*options*/NULL, "TXT.xml"); 


}

void saveXML()
{
mxml_options_t *options = mxmlOptionsNew();
FILE *fd = fopen("TXT.XML","r");

mxml_node_t* parent=mxmlLoadFile(NULL,NULL,fd);

assert(parent!=NULL);

mxmlAdd(parent,MXML_ADD_AFTER,NULL,data);

xml=mxmlNewXML("1.0");
mxml_node_t* datax = mxmlNewElement(xml, "data");
mxmlAdd(datax,MXML_ADD_AFTER,NULL,parent);




int x=mxmlSaveFilename(xml,NULL,"datax.xml"); 

printf("%d",x);


}



