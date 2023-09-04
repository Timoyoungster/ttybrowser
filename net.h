
#define ERROR -1
#define SUCCESS 0
#define READ_BUF_SIZE 10

/*
 * Enum for the search strings of availiable search engines
 * 
 * !!! When reordering/appending/changing this enum !!!
 * !!!   the changes need to be mirrored for the    !!! 
 * !!!        array search_engines in net.c         !!!
 * !!!       and in n_change_search_engine()        !!!
 */
typedef enum { 
  GOOGLE, DUCKDUCKGO
} n_search_engine_t;

/*
 * Requests the given link
 *
 * returns the address of the response string from the server
 * (free needs to be done by the caller!)
 */
char * n_send_request(char *link);

/* 
 * Sends a request to the currently selected search engine
 * search_query will have it's spaces replaced by '+'
 * and will then be appended to the selected engines search string
 * 
 * returns the address of the response string from the server
 * (free needs to be done by the caller!)
 */
char * n_send_search_request(char *search_query);

/*
 * changes the used search engine
 */
void n_change_search_engine(char start_character);
