/* A single file bson implementation
   trageted for 32-bit embedded systems
   spec v1.0 from http://bsonspec.org/spec.html @ Nov 6, 2015
   Ali Alavi
   alialavia@gmail.com
*/
/* How to read from a BSON ?
   Any BSON is represented as a bsondoc struct

*/
#include "mbson.h"
#include <stdlib.h>


int errorno = 0;

BYTE* next_element(BYTE* p, bsontype eltype)
{
  switch (eltype)
    {
        case BT_FLOP64BIT:
          p += 8;
          break;
        case BT_UTF8STRING:
          while (*p++);
          break;
        case BT_EMBEDEDDOC:
          // 4: doc size (int32), 1: size of null terminator
          p += 4 + (int) *p + 1;
          break;
        case BT_ARRAY:
          // 4: doc size (int32), 1: size of null terminator
          p += 4 + (int) *p + 1;
          break;          
        case BT_BINARYDATA:
          // 4: doc size (int32), 1: size of null terminator
          p += 4 + (int) *p + 1;
          break;          
        case BT_UNDEFINED:
          // No Value
          break;
        case BT_OBJECTID:
          // ObjectID is 12 bytes
          p += 12;
          break;
        case BT_BOOLFALSETRUE:
          // one byte 
          p ++;
          break;
        case BT_UTCDATETIME:          
          p += 8;
          break;
        case BT_NULLVALUE:
          // No value
          break;
               
        case BT_INT32BIT:
          p += 4;
          break;
        case BT_TIMESTAMP:
          p += 8;
          break;
        case BT_INT64BIT:
          p += 8;
          break;       
    }
    return p;
}
int elist_nofelements(BYTE* p_elist, int e_listsize)
{
  BYTE* p = p_elist;
  int nof_elements = 0;
  // count elements
  while (p < (p_elist + e_listsize))
  {      
    bsontype btype = (BYTE) *(p++);    
    //skip the name
    while (*p++);            
    p = next_element(p, btype);      
    nof_elements++;
  }
  return nof_elements;
}

void reade_list(BYTE* p_elist, int e_listsize, element* e_list, int nof_elements)
{  
  BYTE* p = p_elist;
  int i = 0;
  while (i < nof_elements)
  {
    if (p > (p_elist + e_listsize))
    {
      errorno = ERR_ELIST_BADSIZEORENDING;
      return ;
    }

    e_list[i].eltype = (BYTE) *(p++);    
    e_list[i].e_name = (char *)p;
    while (*p++);
    #ifdef DEBUG
    printf (">>%d\r\n", (int) (p - p_elist));
    #endif
    e_list[i].value = p;
    p = next_element(p, e_list[i].eltype);      
    i++;
  }    
}

bsondoc bsonread(BYTE* p_bsondoc, int docsize)
{
  BYTE* p = p_bsondoc;
  bsondoc ret;
  // sanity checks

  if ((sizeof p_bsondoc) < 5)  
  {
    errorno = ERR_DOCTOOSMALL;
    return ret;
  }

  if (!docsize)
  {
    docsize = (int) *p_bsondoc;
    #ifdef DEBUG
    printf("SIZE:%d\r\n", docsize);
    #endif
  // last element should always be 0
    if ((p_bsondoc[docsize-1]) != 0x00)
    {
      errorno = ERR_BADSIZEORENDING;
      return ret;
    }
  }
  // continue to e_list
  p += 4;

  // allocate memory for e_list array   
  int nof_elements = elist_nofelements(p, docsize - 5); 
  #ifdef DEBUG
  printf ("NOFEL=%d\r\n", nof_elements);
  #endif
  element* e_list = calloc (sizeof (element), nof_elements);  

  // read into array
  reade_list(p, docsize - 5, e_list, nof_elements); 
  
  ret.size = docsize;
  ret.e_list = e_list;
  ret.nof_elements = nof_elements;
  return ret;
}

void destroy(bsondoc bsondoc)
{
  free(bsondoc.e_list);  
}
