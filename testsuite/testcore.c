#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <inttypes.h>
#include "../binsoncore.c"
/*
// bson test suite
  BYTE test[19]={0x13,0x00,0x00,0x00,BT_INT32BIT,'A','\0',0x01,0x02,0x03,0x04,BT_INT32BIT,'B','\0', 0x05,0x06,0x07,0x08,0x00};
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
    printf ((isarray) ? "[" : "{");
    if (*buffer)
    {
        do 
        {
            if (i>0)
                printf(",");
            printf ("\n");            
            addtabs(level); 
            if (!isarray)
            {                                
                printf("\"%s\": ", getkey(buffer));
            }                    
            switch (gettype(buffer))
            {
                case BT_INT32BIT:
                    printf("%d", asint(getvalue(buffer)));
                    break;
                case BT_FLOP64BIT:
                    printf("%lf", asdouble(getvalue(buffer)));
                    break;
                case BT_BOOLFALSETRUE:
                    printf("%s", (asboolean(getvalue(buffer)) ? "true" : "false"));
                    break;
                case BT_UTF8STRING:
                    printf("\"%s\"", asstring(getvalue(buffer)));
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
            //getchar();
        } while (*(buffer = nextitem(buffer)));           
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

    BYTE* doc = opendoc(content, 0);

    if (errorno != 0)
    {
        fprintf(stderr, "ERROR #%d", errorno);
        exit (1);
    }
    //convert bson to json and print 
    tojson(doc, 1, 1);
    //BYTE* ar0  = lookup(doc, "0", 1);
    //BYTE* ar00 = lookup(opendoc(getvalue(ar0), 0), "description", 11);
    //printf (">>0x%X", (ar00 ? *ar00 : 0));
    //printf ("%s", asstring(getvalue(ar00)));
    BYTE* l = lookuppath(doc, "0/schema/properties/foo/type");
    if (l)
        printf ("%s", asstring(getvalue(l)));
    else
        printf ("NOTFOUND");
    printf("\n");
    free(content);
}