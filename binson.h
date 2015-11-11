#ifndef BINSON_H
#define BINSON_H

#include "binsoncore.h"
#include <unistd.h>
#include <inttypes.h>

/* MACROS */
typedef struct stelement
{
  bsontype eltype;
  char* e_name;  
  BYTE* value;
} stelement;

typedef struct bsondoc 
{
  int size;
  stelement* e_list;
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
int elist_nofelements(BYTE* p_elist, int e_listsize);
void reade_list(BYTE* p_elist, int e_listsize, stelement* e_list, int nof_elements);
bsondoc bsonread(BYTE* p_bsondoc, int docsize);
void destroy(bsondoc bsondoc);

/* Typecast helpers */
float todouble(stelement el);
bsondoc todoc(stelement el);
bsondoc toarray(stelement el);
string tostring(stelement el);
const char* tocharp(stelement el);
binary tobinary(stelement el); 
const BYTE* tobytep(stelement el);
BYTE* toid(stelement el);
bool toboolean(stelement el);
uint64_t todatetime(stelement el);
int32_t toint(stelement el);

#endif
