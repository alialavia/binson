#include "binson.h"
#include <string.h>

BYTE gettype(element buffer)
{  
  return *buffer;
}

element next_element(value p, bsontype eltype)
{
  switch (eltype)
  {
    case BT_FLOP64BIT:
    p += 8;
    break;

    // first 4 bytes in string and binary data indicate length of data, excluding the first four bytes itself
    case BT_JSCODE:
    case BT_SYMBOL:
    case BT_UTF8STRING:        
    p += 4 + *(int32_t *) p;
    break;
    case BT_BINARYDATA:
          // 4 bytes: size (int32), 1 byte: subtype
    p += 4 + *(int32_t *) p + 1;
    break;          
        // first 4 bytes in doc and array indicate length of the whole binary, including the first four bytes itself
    case BT_EMBEDEDDOC:
    p += *(int32_t *) p;
    break;
    case BT_ARRAY:
    p += *(int32_t *) p;
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

/* Returns next keyvalue pair. If there is no item left, p points to the byte after the current element */
element nextitem(element p)
{
  BYTE type = *(p++);
  while (*(p++));
  return next_element(p, type);
}

/* returns value of a keyvalue pair */
value getvalue(element buffer)
{ 
  if (gettype(buffer) == 0)
  {
    errorno = ERR_INVALIDBUFFER;
    return NULL;
  }
  buffer++;
  while (*(buffer++));
  return buffer;
}
int32_t getdoclen(BYTE* buffer)
{
  return *(int32_t *) buffer;
}
/* opens a buffer pointing to the beginning of a bson document */
element opendoc(BYTE* buffer, int len)
{  
  errorno = 0;
  if (!len)
    len = getdoclen(buffer);    
  
  #ifdef DEBUG
  printf("SIZE, end:%d 0X%x\r\n", len, buffer[len-1]);
  #endif  

  if ((buffer[len-1]) != 0x00)
  {
    errorno = ERR_BADSIZEORENDING;
    return NULL;
  }
  
  return buffer + 4;
}

/* */
element lookup(value buffer, char* key, int n)
{    
  /* returns pointer to the beginning of the field with key = key */
  /* buffer points to a doc/array value */
  do
  {    
    #ifdef DEBUG    
    printf("0X%x\r\n", type);    
    printf("key=%s\r\n", getkey(buffer));    
    printf("search=%s\r\n", key);    
    printf("n=%d\r\n", n);    
    #endif    
    if (strncmp(getkey(buffer), key, n) == 0)
    {
      #ifdef DEBUG          
      printf("A\r\n");
      #endif      
      return buffer;        
    }
  } while (*(buffer = nextitem(buffer)));
  return 0;
}

element lookuppath(value buffer, char* path)
{ 
  /* returns pointer to the beginning of the field the path points to */
  /* buffer points to a doc/array value */

  while (*path)
  {
    char* next = path;
    while (*next && *next!='/')
      next++;
    buffer = lookup (buffer, path, (int)(next - path));    
    
    // lookup unsuccessful
    if (!buffer)
      return 0;
    
    // end of path string reached
    if (!*next)
      return buffer;

    #ifdef DEBUG
    printf("type=0X%x", gettype(buffer));
    #endif
    
    // point to the content of the array or embedded doc
    if (gettype(buffer) == BT_ARRAY || gettype(buffer) == BT_EMBEDEDDOC)
      buffer = opendoc(getvalue(buffer), 0); 
    
    #ifdef DEBUG
    printf ("B=%s\r\n", next);
    #endif      

    #ifdef DEBUG
    printf ("%s\r\n", next);
    getchar();
    #endif    

    next++;
    path = next;    
  }
  return 0;
}

char* getkey(element buffer)
{  
  return (char *) (buffer + 1);
}

/* convert a primitive value to its corresponding c type */
int32_t asint(value v) { return *(int32_t *) (v); }
double asdouble(value v) { return  *(double *) (v); }
bool asboolean(value v) { return *(BYTE *) (v); }
char* asstring(value v) { return  (char *) ((v) + 4); }
