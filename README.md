# binson: A minimum bson implementation for embedded systems

This implementation targets embedded devices, thus is focused on minimum memory and flash footprints. 

## binson

You can use binson.h for minimum memory footprint. It doesn't allocate any dynamic memory. 

Here is how it works:

```c
/*
Assume we load bson representation of the following json 
{ [{ID : 123, Personal: {Name: 'Mike', Age: 25}}, {ID : 124, Personal: 
{Name: 'Alex', Age: 30}}] }

for example from a file:
#define FILENAME "employees.bson"

struct stat info;
stat(FILENAME, &info);
FILE* fp = fopen(FILENAME, "rb");
BYTE* content = malloc(info.st_size);
*/

BYTE* doc = opendoc(content, 0); //Load content into a doc. Second argument is the length of the byte array. Leave it as 0 if unknown and it will be automatically determined.

if (errorno != 0)   // You can check errorno after every operation 
{
    /* Handle the error */
}

BYTE* id0 = lookuppath(doc, "0/ID");
printf("ID0 = %d", asint(id0));
BYTE* name0 = lookuppath(doc, "0/Personal/Name");
printf("Name0 = %s", asstring(name0));

BYTE* id1 = lookuppath(doc, "1/ID");
printf("ID1 = %d", asint(id1));
BYTE* name1 = lookuppath(doc, "1/Personal/Name");
printf("Name1 = %s", asstring(name1));
BYTE* age1 = lookuppath(doc, "1/Personal/Age");
printf("Age1 = %s", asstring(age1));

// If the key doesn't exist it returns zero
BYTE* city1 = lookuppath(doc, "1/Personal/City");
if (city1)  
    printf("City1 = %s", asstring(city));
    
```
For a more ellaborate example take a look at testsuite/testbison.c.

## Dependencies
* binson.h uses inttypes.h

## Testsuite

I converted MongoDB's binary test suits (test-cases/bson) using MongoDB's bson-to-json (both from [libbson](https://github.com/mongodb/libbson/)) and saved the results under test-cases/json. Then, using testsuite/testbison, we test if bison also get us the same jsons. We can run all the tests using the testsuit/test.sh.

#TODO
* Fix errors in test_res.out

* Analyze memory footprint

* Write lookup functions for binsontree.h


