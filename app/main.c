#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/getopt_core.h>

int main (int argc, char **argv) {
  int index;
  int c; 
  opterr = 0;

  if(setenv("MY_PROMPT", "foo>", 1) != 0){
    fprintf(stderr, "my prompt failed.\n");
    exit(-1);
  }

  while ((c = getopt (argc, argv, "abc:")) != -1)
    switch (c)
      {
        case 'v':
        // print program version
        printf("Version 1.0");
        exit(0);
        break;
      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
      }

  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);
  return 0;
}