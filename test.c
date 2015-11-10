#include <bson.h>
#include <stdlib.h>
#include <stdio.h>
//#include "mbson.h"
#include "mbson.c"
/*
// bson test suite
  BYTE test[19]={0x13,0x00,0x00,0x00,BT_INT32BIT,'A','\0',0x01,0x02,0x03,0x04,BT_INT32BIT,'B','\0', 0x05,0x06,0x07,0x08,0x00};
  */
void test(BYTE* doc)
{
  
  bsondoc output = bsonread(doc, 0);  
  int i = 0;
  for (i = 0; i < output.nof_elements; i++)
  {
    element el = output.e_list[i];
    printf("0x%x:%s:", el.eltype, el.e_name);
    switch (el.eltype)
    {
        case BT_INT32BIT:
            printf("%d", asint(el));
            break;
        case BT_FLOP64BIT:
            printf("%d", asfloat(el));
            break;
    }
  }
  
  if (errorno)
    printf ("ERROR #%d\r\n", errorno);

  destroy(output);
  //return 0;
}
int
main (int   argc,
      char *argv[])
{
   bson_json_reader_t *reader;
   bson_error_t error;
   const char *filename;
   bson_t doc = BSON_INITIALIZER;
   int i;
   int b;

   /*
    * Print program usage if no arguments are provided.
    */
   if (argc == 1) {
      fprintf (stderr, "usage: %s FILE...\n", argv[0]);
      return 1;
   }

   /*
    * Process command line arguments expecting each to be a filename.
    */
   for (i = 1; i < argc; i++) {
      filename = argv[i];

      /*
       * Open the filename provided in command line arguments.
       */
      if (0 == strcmp (filename, "-")) {
         reader = bson_json_reader_new_from_fd (STDIN_FILENO, false);
      } else {
         if (!(reader = bson_json_reader_new_from_file (filename, &error))) {
            fprintf (stderr, "Failed to open \"%s\": %s\n",
                     filename, error.message);
            continue;
         }
      }

      /*
       * Convert each incoming document to BSON and print to stdout.
       */
      while ((b = bson_json_reader_read (reader, &doc, &error))) {
         if (b < 0) {
            fprintf (stderr, "Error in json parsing:\n%s\n", error.message);
            abort ();
         }

         /*
         if (fwrite (bson_get_data(&doc), 1, doc.len, stdout) != doc.len) {
            fprintf (stderr, "Failed to write to stdout, exiting.\n");
            exit (1);
         }*/
        
            test(bson_get_data(&doc));
            bson_reinit (&doc);
      }

      bson_json_reader_destroy (reader);
      bson_destroy (&doc);
   }

   return 0;
}
