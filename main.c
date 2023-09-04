#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "net.h"

#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[]) {

  /* read in the options */
  int is_url = FALSE;
  int opt;
  while ((opt = getopt(argc, argv, "le:")) != -1) {
    switch (opt) {
      case 'l':
        is_url = TRUE;
        break;
      case 'e':
        n_change_search_engine(optarg[0]);
    }
  }

  /* 
   * if the input_str isn't at the end use the first argument
   * (obviously if no search string is given it will search
   *  for something like "-e" => not ideal, wontfix for now)
   */
  if (optind == argc) {
    optind = 1;
  }

  char *input_str = argv[optind];

  if (is_url == TRUE) {
    n_send_request(input_str);
  }
  else {
    n_send_search_request(input_str);
  }

  // show page

  // free(page);
}
