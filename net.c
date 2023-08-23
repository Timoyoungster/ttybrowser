#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "net.h"

static n_search_engine_t selected_search_engine = GOOGLE;

/*
 * Array holding the search query strings for each search engine
 * 
 * !!! When reordering/appending/changing this array !!!
 * !!!    the changes need to be mirrored for the    !!! 
 * !!!        enum n_search_engine_t in net.h        !!!
 * !!!        and in n_change_search_engine()        !!!
 */
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
  printf("Opening link: %s\n", link);

  char site[] = "142.250.217.132"; /* get through getaddrinfo() */
  struct sockaddr_in site_addr = { 
    .sin_family = AF_INET, 
    .sin_port = htons(80) 
  };
  char buff[10] = {0};
  char req[] = "GET / HTTP/1.1\r\nHost:www.google.com\r\n\r\n";
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) return -1;

  if (inet_pton(AF_INET, site, &site_addr.sin_addr) < 0) {
    fprintf(stderr, "invalid address\n");
    return -1;
  }

  if (connect(sock, (struct sockaddr*) &site_addr, sizeof(site_addr)) < 0) {
    fprintf(stderr, "connection failed\n");
    return -1;
  }

  send(sock, req, strlen(req), 0);

  while (read(sock, buff, 8) == 8) {
    printf("%s", buff);
  }


  free(link);
  return "Request sending not yet implemented!";
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
  int engine_string_len = strlen(search_engines[selected_search_engine]) + 1;
  int link_len = sizeof(char) 
      * strlen(search_query) 
      * engine_string_len; /* no +1 because engine_string_len includes it */

  char *link = malloc(link_len);

  /* 
   * copy the engines query string to the link 
   * and append the search string
   */
  strlcpy(link, search_engines[selected_search_engine], engine_string_len);
  strlcat(link, search_query, engine_string_len + strlen(search_query));

  return n_send_request(link);
}

/*
 * changes the used search engine
 */
void n_change_search_engine(char start_character) {
  char *new_search_engine_name = malloc(255);
  switch (start_character) {
    case 'g':
    case 'G':
      selected_search_engine = GOOGLE;
      strlcpy(new_search_engine_name, "Google", 7);
      break;
    case 'd':
    case 'D':
      selected_search_engine = DUCKDUCKGO;
      strlcpy(new_search_engine_name, "DuckDuckGo", 11);
      break;
  }
  printf("New search engine is %s\n", new_search_engine_name);
}
