#include <stdio.h>
#include "net.h"

static n_search_engine_t selected_search_engine = GOOGLE;

static const char * const search_engines[] = {
  "https://www.google.com/search?q=", /* GOOGLE */
  "https://duckduckgo.com/?q="      /* DUCKDUCKGO */
};

/*
 * Requests the given link
 *
 * returns the address of the response string from the server
 * (free needs to be done by the caller!)
 */
char * n_send_request(char *link) {
  return "Not yet implemented!";
}

/* 
 * Sends a request to the currently selected search engine
 * search_query will have it's spaces replaced by '+'
 * and will then be appended to the selected engines search string
 * 
 * returns the address of the response string from the server
 * (free needs to be done by the caller!)
 */
char * n_send_search_request(char *search_query) {
  return "Not yet implemented!";
}

/*
 * changes the used search engine
 */
void n_change_search_engine(char start_character) {
  switch (start_character) {
    case 'g':
    case 'G':
      selected_search_engine = GOOGLE;
      break;
    case 'd':
    case 'D':
      selected_search_engine = DUCKDUCKGO;
      break;
  }
  printf("New search engine is number %d\n", selected_search_engine);
}
