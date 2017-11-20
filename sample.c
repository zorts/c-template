#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

static
void usage(int argc, char**argv){
  fprintf(stderr,
          "usage: %s \n"
          "  -h    get usage help \n"
          "  -v    verbose \n"
          "  -d    debug \n"
          "  -e    <n> -- desired exit code \n"
          " \n"
          , 
          argv[0]);
}

int main(int argc, char**argv){
  if (argc == 1){
    usage(argc, argv);
    return 1;
  }
  opterr = 0; /* disable auto error reporting */
  char opt = 0;
  /* These copies are needed because optind and optarg aren't
     necessarily visible to debuggers, and you often want them. */
  int myoptind = 1;
  char* myoptarg = 0;
  bool verbose = false;
  bool debug = false;
  const char* exitCode = 0;

  while (((char) -1) != (opt = (char) getopt(argc, argv, "vdhe:"))){
    myoptind = optind;
    myoptarg = optarg;

    switch(opt){

    case 'v':
      verbose = true;
      break;

    case 'd':
      debug = true;
      verbose = true;
      break;

    case 'h':
      usage(argc, argv);
      return 1;
      break;

    case 'e':
      exitCode = myoptarg;
      break;

    default:
      {
        char erropt = optopt;
        fprintf(stderr, "unrecognized option '%c'\n", erropt);
      }
      break;
    }
  }

  if (myoptind < argc){
    fprintf(stderr, "unused arguments:");
    while (myoptind < argc){
      fprintf(stderr, " %s", argv[myoptind++]);
    }
    fprintf(stderr, "\n");
    usage(argc, argv);
    return 1;
  }

  int returnCode = (0 != exitCode ? atoi(exitCode) : 0);

  if (debug){
    fprintf(stderr, "Exit code string is \"%s\"; integer value is %d\n",
            (0 != exitCode ? exitCode : ""), returnCode);
  }

  if (verbose){
    fprintf(stderr, "Exiting with %d\n", returnCode);
  }

  return returnCode;
}
