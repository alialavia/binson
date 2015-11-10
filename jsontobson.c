#include <bson.h>
#include <stdlib.h>

int
main (int   argc,
      char *argv[])
{
   bson_json_reader_t *reader;
   bson_error_t error;
   
   bson_t doc = BSON_INITIALIZER;   
   int b;

   /*
    * Print program usage if no arguments are provided.
    */
   if (argc == 1) {
      fprintf (stderr, "usage: %s JSONFILE\n", argv[0]);
      return 1;
   }
  printf ("%d\r\n", strlen(argv[1]));

  char *filename = malloc(strlen(argv[1]) + 1);
  strcpy(filename, argv[1]);
  filename[strlen(argv[1])+1] = 0;

    if (0 == strcmp (filename, "-")) {
       reader = bson_json_reader_new_from_fd (STDIN_FILENO, false);
    } else {
       if (!(reader = bson_json_reader_new_from_file (filename, &error))) {
          fprintf (stderr, "Failed to open \"%s\": %s\n",
          filename, error.message);
          exit (1);
       }
    }

    filename[strlen(filename)-4] = 'b'; // filename.json -> filename.bson  
    FILE* outp = fopen(filename, "wb");
        /*
     * Convert the document to BSON and save it as a binary
     */
    while ((b = bson_json_reader_read (reader, &doc, &error))) {
       if (b < 0) {
          fprintf (stderr, "Error in json parsing:\n%s\n", error.message);
          abort ();
       }

      
       if (fwrite (bson_get_data(&doc), 1, doc.len, outp) != doc.len) {
          fprintf (stderr, "Failed to write to output file, exiting.\n");
          exit (1);
       }            
       
    }
    fclose(outp);
    bson_json_reader_destroy (reader);
    bson_destroy (&doc);
 

   return 0;
}
