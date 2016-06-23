# binson: A minimum bson implementation for embedded systems

This implementation targets embedded devices, thus is focused on minimum memory and flash footprints. 

## binsoncore

You can use binsoncore.h for minimum memory footprint. It doesn't allocate any dynamic memory. 

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
For a more ellaborate example take a look at testsiute/testcore.c.

## binsontree
You can also use binsontree.h which creates more advanced data structure and should offer faster lookup. You need to allocate and dellocate dynamic memory though, and it's not being maintained as offen as binsoncore. So prefer to use binsoncore at this moment.

For a sample usage take a look at testsuite/test.c.

Binsontree creates a dynamic array. Each element has a type and two pointers to key and value pairs in the bson. 
Type field is 1 byte, and key and value pointers are 4 bytes each (on a 32-bit address bus system), hence total dynamic memory allocated would be (1+4+4 = 9) x number of elements in bson.

The array can be used to iterate over elements. Helpers functions toxxx (toboolean, tostring, ...) can be used for type casting element values.
A simple bson to json is provided both as an example as to be used for running tests.

## Dependencies
* binsoncore.h uses inttypes.h
* binsontree.h uses stdlib as well

## Testsuite

read_value.c uses MongoDB's [libbson](https://github.com/mongodb/libbson/) to convert json to bson. You don't need it unless you want to run test cases.
Test cases are from [JSON-Schema-Test-Suite](https://github.com/json-schema/JSON-Schema-Test-Suite)

#TODO
* Run all test cases

* Analyze memory footprint

* Write lookup functions for binsontree.h


