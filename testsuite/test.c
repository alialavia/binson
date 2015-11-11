#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <inttypes.h>
#include "../binson.h"

void addtabs(int l)
{
    int j = 0;
    for (j = 0;j < l; j++)
        printf("\t");
}

void tojson(bsondoc doc, int level, int isarray)
{  
    int i = 0;
    bsondoc d;
    printf ((isarray) ? "[" : "{");
    
    for (i = 0; i < doc.nof_elements; i++)
    {

        stelement el = doc.e_list[i];
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
                printf("\"%d\"", toint(el));
                break;
            case BT_FLOP64BIT:
                printf("\"%lf\"", todouble(el));
                break;
            case BT_BOOLFALSETRUE:
                printf("\"%s\"", (toboolean(el) ? "true" : "false"));
                break;
            case BT_UTF8STRING:
                printf("\"%s\"", tostring(el).str);
                break;
            case BT_EMBEDEDDOC:
            case BT_ARRAY:
                d = todoc(el);
                tojson(d, level + 1, (el.eltype == BT_ARRAY));
                destroy(d);
                break;
            default:
                printf("\"__NOTIMPLEMENTED__\"");
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
    if (errorno != 0)
    {
        fprintf(stderr, "ERROR #%d", errorno);
        exit (1);
    }
    tojson(output, 1, 0);
    printf("\n");
    destroy(output);
    free(content);
}