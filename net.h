/*
 * Enum for the search strings of availiable search engines
 */
enum n_search_engine { 
  GOOGLE, DUCKDUCKGO, MAX_SEARCH_ENGINE
};

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
void n_change_search_engine(enum n_search_engine st);
