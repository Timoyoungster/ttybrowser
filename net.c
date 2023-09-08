#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
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
 * Iterates through the input string (starting at start_at) and 
 * returns the nth-occurance of c (search character)
 *
 * (nth is zero based; find first '.' => nth=1)
 * (start_at is zero based as well)
 */
static int get_inx_start_at(char *str, char c, 
    int nth, int start_at) 
{
  int count = 0;
  for (int i = start_at; i < strlen(str); i++) {
    if (str[i] == c) {
      if (nth == count++) {
        return i;
      }
    }
  }
  return ERROR;
}

/*
 * Iterates through the input string and 
 * returns the nth-occurance of c (search character)
 *
 * (nth is zero based; find first '.' => nth=1)
 */
static int get_inx(char *str, char c, int nth) {
  return get_inx_start_at(str, c, nth, 0);
}

/*
 * Builds the request string and returns it
 * (free needs to be done by the caller)
 */
static char * build_get_request(char *http_version,
    char *host, char *data) {
  char method[] = "GET";
  char *result = malloc(sizeof(char)
      * (
          3 + 1 + strlen(data) + 1 + 
          strlen(http_version) + 7 + strlen(host) + 5
        )
    );
  strcpy(result, method);
  strcat(result, " ");
  strcat(result, data);
  strcat(result, " ");
  strcat(result, http_version);
  strcat(result, "\r\nHost:");
  strcat(result, host);
  strcat(result, "\r\n\r\n");
  return result;
}

/*
 * Requests the given link
 *
 * returns the address of the response string from the server
 * or NULL on error
 * (free needs to be done by the caller!)
 */
char * n_send_request(char *link) {
  printf("Opening link: %s\n", link);

  /*
   * get the start and end of the host
   * (start incl; end excl)
   */
  int first_dot = get_inx(link, '.', 0);
  if (first_dot < 3) {
    return NULL;
  }
  int host_start = first_dot - 3;
  int host_end;
  if (link[0] == 'h') {
    host_end = get_inx(link, '/', 2);
  }
  else {
    host_end = get_inx(link, '/', 0);
  }

  /*
   * prepare request string parts
   */
  char http_version[] = "HTTP/1.1";
  char *host = malloc(host_end - host_start + 1);
  char *args = malloc(strlen(link) - host_end + 1);

  strlcpy(host, &link[host_start], host_end - host_start + 1);
  strlcpy(args, &link[host_end], strlen(link) - host_end + 1);

  /*
   * create the request string
   *
   * "GET / HTTP/1.1\r\nHost:www.google.com\r\n\r\n"
   */
  char *req = build_get_request(http_version, host, args);

  /*
   * create address structs
   */
  struct addrinfo hints;
  struct addrinfo *result, *rp;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  /*
   * get the availiable addresses
   */
  int error = getaddrinfo(host, "https", &hints, &result);
  if (error != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
    return NULL;
  }

  /*
   * create the socket with the first address that works
   */
  int sock;
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

    if (sock == -1)
      continue;

    if (connect(sock, rp->ai_addr, rp->ai_addrlen) == 0)
      break;

    close(sock);
  }

  freeaddrinfo(result);

  if (rp == NULL) {
    fprintf(stderr, "Could not connect to host %s\n", host);
    return NULL;
  }

  /*
   * send the request
   */
  error = send(sock, req, strlen(req), 0);
  if (error == -1) {
    fprintf(stderr, "No data sent!");
    close(sock);
    return NULL;
  }

  /*
   * read the response
   */
  char response_buf[READ_BUF_SIZE] = {0};
  while ((error = read(sock, response_buf, READ_BUF_SIZE)) > 0) {
    printf("%s", response_buf);
  }

  if (error == -1) {
    fprintf(stderr, "Couldn't read response stream!");
    close(sock);
    return NULL;
  }

  close(sock);
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
   * replace spaces
   */
  for (int i = 0; i < strlen(search_query); i++) {
    if (search_query[i] == ' ')
      search_query[i] = '+';
  }

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
