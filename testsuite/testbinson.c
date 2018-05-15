#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <inttypes.h>
#include "binson.h"
/*
How the test works
*/
void addtabs(int l)
{
    int j = 0;
    for (j = 0;j < l; j++)
        printf("    ");
}

void tojson(BYTE* buffer, int level, int isarray)
{  
    int i = 0;    
    printf ((isarray) ? "[ " : "{ ");
    if (*buffer)
    {
        do 
        {
            if (i>0)
                printf(", ");
            //addtabs(level); 
            if (!isarray)
            {                                
                printf("\"%s\" : ", getkey(buffer));
            }                    
            switch (gettype(buffer))
            {


                case BT_INT32BIT:
                    printf("%d", asint(getvalue(buffer)));
                    break;
                case BT_FLOP64BIT:
                    printf("%0.2lf", asdouble(getvalue(buffer)));
                    break;
                case BT_BOOLFALSETRUE:
                    printf("%s", (asboolean(getvalue(buffer)) ? "true" : "false"));
                    break;
                case BT_SYMBOL:    
                case BT_JSCODE:
                case BT_UTF8STRING:
                    printf("\"%s\"", asstring(getvalue(buffer)));
                    break;
                case BT_NULLVALUE:
                    printf("null");
                    break;
                case BT_INT64BIT:
                    printf("%llu", (unsigned long long)*(getvalue(buffer)));
                    break;
                case BT_EMBEDEDDOC:
                    tojson(opendoc(getvalue(buffer), 0), level + 1, 0);                                
                    break;
                case BT_ARRAY:                 
                    tojson(opendoc(getvalue(buffer), 0), level + 1, 1);                
                    break;

                default:
                    printf(" \"(0x%X) __NOTIMPLEMENTED__\"", gettype(buffer));
            }
            
            i++;
        } while (*(buffer = nextitem(buffer)));           
        //addtabs(level - 1);
    }
       
    printf ((isarray) ? " ]" : " }");
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

    BYTE* doc = opendoc(content, 0);

    if (errorno != 0)
    {
        fprintf(stderr, "ERROR #%d", errorno);
        exit (1);
    }
    //convert bson to json and print 
    tojson(doc, 1, 0);

    /*BYTE* l = lookuppath(doc, "0/schema/properties/foo/type");
    if (l)
        printf ("%s", asstring(getvalue(l)));
    else
        printf ("NOTFOUND");
    */
    printf("\n");
    
    free(content);
}