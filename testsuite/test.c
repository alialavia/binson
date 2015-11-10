#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <inttypes.h>
#include "mbson.c"
/*
// bson test suite
  BYTE test[19]={0x13,0x00,0x00,0x00,BT_INT32BIT,'A','\0',0x01,0x02,0x03,0x04,BT_INT32BIT,'B','\0', 0x05,0x06,0x07,0x08,0x00};
  */
void addtabs(int l)
{
    int j = 0;
    for (j = 0;j < l; j++)
        printf("\t");
}

void tojson(bsondoc doc, int level, int isarray)
{  
    int i = 0;
    
    printf ((isarray) ? "[" : "{");
    
    for (i = 0; i < doc.nof_elements; i++)
    {

        element el = doc.e_list[i];
        if (i > 0)
            printf(", ");

        if (!isarray)
        {
            printf ("\n");
            addtabs(level); 
            printf("\"%s\": ", el.e_name);
        }        
            
        switch (el.eltype)
        {
            case BT_INT32BIT:
                printf("\"%d\"", asint(el));
                break;
            case BT_FLOP64BIT:
                printf("\"%lf\"", asdouble(el));
                break;
            case BT_UTF8STRING:
                printf("\"%s\"", asstring(el).str);
                break;
            case BT_EMBEDEDDOC:
            case BT_ARRAY:
                tojson(asdoc(el), level + 1, (el.eltype == BT_ARRAY));
                break;
            default:
                printf("\"__NOT_IMPL__\"");
        }
    }   
    if (!isarray)
    {
        printf ("\n");
        addtabs(level - 1); 
    }
    printf ((isarray) ? "]" : "}");
    if (errorno)
        printf ("ERROR #%d\n", errorno);    
}

int
main (int   argc,
      char *argv[])
{
    if (argc == 1) {
      fprintf (stderr, "usage: %s FILE\n", argv[0]);
      return 1;
    }
    const char *filename = argv[1];
    struct stat info;
    stat(filename, &info);
    FILE* fp = fopen(filename, "rb");
    BYTE* content = malloc(info.st_size);
    if (!content)
    {
    fprintf(stderr, "Not enough memory to open file\n");
    exit (1);
    }
    fread(content, info.st_size, 1, fp);
    fclose(fp);
    bsondoc output = bsonread(content, 0);
    tojson(output, 1, 0);
    printf("\n");
    destroy(output);
}