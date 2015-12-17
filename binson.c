//
// Created by ali on 12/15/15.
//
#include "jsonreader.h"
#include <stdio.h>
#include <stdlib.h>

#define STRSIZE 1000000

long fsize(FILE* fp)
{
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}
int main(int argc, char *argv[])
{


    if (argc < 2)
        return -1;
    char* buffer = malloc(STRSIZE);

    if (!buffer)
    {
        fprintf(stderr, "Cannot allocate memory. Exiting.\r\n");
        return 0;
    }
    FILE *fp = stdin;
    if (argc == 3) {
        fp = fopen(argv[2], "r");
        if (!fp)
        {
            fprintf(stderr, "Cannot open file. Exiting.\r\n");
            return 0;
        }
    }

    long size = fsize(fp);
    fread(buffer, 1, size, fp);

    switch (*argv[1]) {
        case 'n':
            printf("%d\r\n", matchJSONNumber(&buffer));
            break;
        case 's':
            printf("%d\r\n", json_readstring(&buffer));
            break;
        case 'j':
            printf("%d\r\n", read_value(&buffer));
            printf ("%s", buffer);
            break;
    }
    fclose(fp);
    return 0;
}