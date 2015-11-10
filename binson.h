#ifndef BINSON_H
#define BINSON_H

#include <unistd.h>
#include <inttypes.h>

/* MACROS */
#define ERR_DOCTOOSMALL 1
#define ERR_BADSIZEORENDING 2
#define ERR_ELIST_BADSIZEORENDING 3
#define ERR_NOTENOUGHMEMORY 4

#define BYTE uint8_t
#define bool uint8_t


typedef enum bsontype {
  BT_FLOP64BIT = 0x01,
  BT_UTF8STRING = 0x02,
  BT_EMBEDEDDOC = 0x03,
  BT_ARRAY = 0x04,
  BT_BINARYDATA = 0x05,
  BT_UNDEFINED = 0x06,
  BT_OBJECTID = 0x07,
  BT_BOOLFALSETRUE = 0x08,
  BT_UTCDATETIME = 0x09,
  BT_NULLVALUE = 0x0A,
 //0x0B..0x0F of no use in this implementation,
  BT_INT32BIT = 0x10,
  BT_TIMESTAMP = 0x11,
  BT_INT64BIT = 0x12,
  // of no use,
  BT_MINKEY = 0xFF,
  BT_MAXKEY = 0x7F
} bsontype;

typedef enum bsonsubtype {
 // subtypes,
  ST_GENERIC = 0x00,
  ST_FUNCTION = 0x01,
  ST_BINARYOLD = 0x02,
  ST_UUIDOLD = 0x03,
  ST_UUID = 0x04,
  ST_MD5 = 0x05,
  ST_USERDEFINED = 0x80
} bsonsubtype;

typedef struct element
{
  bsontype eltype;
  char* e_name;  
  BYTE* value;
} element;

typedef struct bsondoc 
{
  int size;
  element* e_list;
  int nof_elements;
} bsondoc;

typedef struct string
{
  int size;
  char *str;
} string;

typedef struct binary
{
  int size;
  BYTE* data;
} binary;

/* Core functions */
BYTE* next_element(BYTE* p, bsontype eltype);
int elist_nofelements(BYTE* p_elist, int e_listsize);
void reade_list(BYTE* p_elist, int e_listsize, element* e_list, int nof_elements);
bsondoc bsonread(BYTE* p_bsondoc, int docsize);
void destroy(bsondoc bsondoc);

/* Typecase helpers */
float asdouble(element el);
bsondoc asdoc(element el);
bsondoc asarray(element el);
string asstring(element el);
const char* ascharp(element el);
binary asbinary(element el); 
const BYTE* asbytep(element el);
BYTE* asid(element el);
bool asboolean(element el);
uint64_t asdatetime(element el);
int32_t asint(element el);

#endif
