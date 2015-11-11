/* A single file bson implementation
   trageted for 32-bit embedded systems
   spec v1.0 from http://bsonspec.org/spec.html @ Nov 6, 2015
   Ali Alavi
   alialavia@gmail.com
*/
/* How to read from a BSON ?
   Any BSON is represented as a bsondoc struct

*/

#include "binson.h"


#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

int elist_nofelements(BYTE* p_elist, int e_listsize)
{
  BYTE* p = p_elist;
  int nof_elements = 0;
  // count elements
  while (p < (p_elist + e_listsize))
  {      
    #ifdef DEBUG
    BYTE* temp = p;
    #endif
    bsontype btype = (BYTE) *(p++);    
    //skip the name
    while (*p++);            
    #ifdef DEBUG
    printf("strlen of %s= %d, %x\r\n", (char *)(temp + 1), p - temp, *p);
    #endif
    p = next_element(p, btype);      
    #ifdef DEBUG
    printf("N-C of %s= %d\r\n", (char *)(temp + 1), p - temp);
    printf("N-C of %s= %d\r\n", (char *)(temp + 1), p - temp);
    #endif
    nof_elements++;
  }
  return nof_elements;
}

void reade_list(BYTE* p_elist, int e_listsize, stelement* e_list, int nof_elements)
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
    printf (">>offset=%d\r\n", (int) (p - p_elist));
    #endif
    e_list[i].value = p;
    p = next_element(p, e_list[i].eltype); 
    #ifdef DEBUG
    printf (">>nextstartswith=%x\r\n", *p);
    #endif     
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
    docsize = *(int32_t *) p_bsondoc;
    #ifdef DEBUG    
    printf("SIZE:%d\r\n", docsize);
    #endif
  // last stelement should always be 0
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
  stelement* e_list = calloc (sizeof (stelement), nof_elements);  
  if (!e_list)
  {
    errorno = ERR_NOTENOUGHMEMORY;
    return;
  }
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

float todouble(stelement el)
{
  return *(double *) el.value;
}

bsondoc todoc(stelement el)
{
  return bsonread(el.value, 0);
}

bsondoc toarray(stelement el)
{
  return bsonread(el.value, 0);
}

string tostring(stelement el)
{
  string ret;
  ret.size = *(int *)el.value;
  ret.str = (char *)(el.value + 4);
  return ret;
}

const char* tocharp(stelement el)
{
  return (char *)(el.value + 4);  
}

binary tobinary(stelement el)
{
  binary ret;
  ret.size = *(int *)el.value;
  ret.data = (BYTE *) (el.value + 4);
  return ret;
}

const BYTE* tobytep(stelement el)
{
  return (el.value + 4);  
}

BYTE* toid(stelement el)
{  
  return el.value;
}

bool toboolean(stelement el)
{  
  return *(el.value);
}

uint64_t todatetime(stelement el)
{  
  return *(int64_t *) el.value;
}

int32_t toint(stelement el)
{
  return *(int32_t *) el.value;
}