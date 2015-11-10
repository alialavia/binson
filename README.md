# binson: A minimum bson implementation for embedded systems

This implementation targets embedded devices, thus is focused on minimum memory and flash footprints.

Binson creates a dynamic array. Each element has a type and two pointers to key and value pairs in the bson. 
Type field is 1 byte, and key and value pointers are 4 bytes each (on a 32-bit address bus system), hence total dynamic memory allocated would be (1+4+4 = 9) x number of elements in bson.

The array can be used to iterate over elements. Helpers functions asxxx (asboolean, asstring, ...) can be used for type casting element values.
A simple bson to json is provided both as an example as to be used for running tests.

## Dependencies
Nothing but stdlib and inttypes

## Testsuite

jsontobson.c uses MongoDB's [libbson](https://github.com/mongodb/libbson/) to convert json to bson. You don't need it unless you want to run test cases.
Test cases are from [JSON-Schema-Test-Suite](https://github.com/json-schema/JSON-Schema-Test-Suite)

#TODO

Run all test cases
