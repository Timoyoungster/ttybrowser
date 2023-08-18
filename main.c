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

  char *input_str = argv[0];
  char *page;

  if (is_url == TRUE) {
    page = n_send_request(input_str);
  }
  else {
    page = n_send_search_request(input_str);
  }

  printf("%s\n", input_str);

  // show page

  // free(page);
}
