//
// Created by ali on 12/15/15.
//
#include "jsonreader.h"
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{

    if (argc < 3)
        return -1;
    char *b = argv[2];
    switch (*argv[1]) {
        case 'n':
            printf("%d\r\n", matchJSONNumber(&b));
            break;
        case 's':
            printf("%d\r\n", json_readstring(&b));
            break;
        case 'j':
            printf("%d\r\n", read_value(&b));
            break;
    }
    return 0;
}