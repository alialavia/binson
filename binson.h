#ifndef BINSON_H
#define BINSON_H
int errorno;
#include <unistd.h>
#include <inttypes.h>

#define ERR_DOCTOOSMALL 1
#define ERR_BADSIZEORENDING 2
#define ERR_ELIST_BADSIZEORENDING 3
#define ERR_NOTENOUGHMEMORY 4
#define ERR_INVALIDBUFFER 5

#define BYTE uint8_t
#define bool uint8_t

/* define aliases for distinguishing between functions */

typedef BYTE* value;
typedef BYTE* key;
typedef BYTE* element;
typedef BYTE* e_list;


typedef enum bsontype {
  BT_FLOP64BIT = 0x01,
  BT_UTF8STRING = 0x02,
  BT_EMBEDEDDOC = 0x03,
  BT_ARRAY = 0x04,
  BT_BINARYDATA = 0x05,
  BT_UNDEFINED = 0x06, // Deprecated
  BT_OBJECTID = 0x07,
  BT_BOOLFALSETRUE = 0x08,
  BT_UTCDATETIME = 0x09,
  BT_NULLVALUE = 0x0A,
  //0x0B TODO,
  //BT_DBPointer = 0x0C // Deprecated
  BT_JSCODE = 0x0D,
  BT_SYMBOL = 0x0E, // Deprecated
  //0x0F TODO,
  BT_INT32BIT = 0x10,
  BT_TIMESTAMP = 0x11,
  BT_INT64BIT = 0x12,
  // of no use,
  // BT_MINKEY = 0xFF,
  // BT_MAXKEY = 0x7F
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

/* core functions */

/* return the element after value p. Value p has type eltype */
element next_element(value p, bsontype eltype);

/* return the element after keyvalue pair p */
element nextitem(element p);

/* getvalue of element buffer */
value getvalue(element buffer);

/* returns first element of the array/doc */
element opendoc(BYTE* buffer, int len);

/* lookup in an array/doc value for a specific key 'key'. key string has lenght n */
element lookup(value buffer, char* key, int n);

/* gets key of a keyvalue pair */
char* getkey(element buffer);

/* get type of a keyvalue pair's value */
BYTE gettype(element buffer);

/* get length of array/doc */
int32_t getdoclen(BYTE* buffer);


/* convert a primitive value to its corresponding c type */

int32_t asint(value v);

double asdouble(value v);
bool asboolean(value v);
char* asstring(value v);
#endif