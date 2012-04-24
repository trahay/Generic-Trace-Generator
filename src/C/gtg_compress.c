
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "GTGCompress.h"


char* input_filename;
char* output_filename;

static void
usage (int argc  __attribute__((unused)), char **argv)
{
  fprintf (stderr,
	   "Usage : %s -i input_filename [-o output_filename] \n",
	   argv[0]);
}

static void
parse_args (int argc, char **argv)
{
  int i;
  input_filename = NULL;
  for (i = 1; i < argc; i++)
    {
      if (strcmp (argv[i], "-o") == 0) {
	output_filename = argv[++i];
      } else if (strcmp (argv[i], "-i") == 0) {
	if(input_filename) {
	  usage (argc, argv);
	  exit (-1);
	}
	input_filename = argv[++i];
      } else {
	if(input_filename) {
	  usage (argc, argv);
	  exit (-1);
	}
	input_filename = argv[i];
      }
    }
}

int main(int argc, char**argv)
{
  parse_args(argc, argv);

  if(!input_filename) {
    fprintf(stderr, "please provide an input file and an output file\n");
    exit(-1);
  }

  if(!output_filename) {
    asprintf(&output_filename, "%s.z", input_filename);
  }

  printf("compressing file %s into file %s\n", input_filename, output_filename);

  FILE *fin=fopen(input_filename, "r");
  FILE *fout=fopen(output_filename, "w");

  z_stream z;
  gtg_compress_init(&z, 1);
  gtg_compress_f2f(&z, fin, fout);

  return 0;
}
