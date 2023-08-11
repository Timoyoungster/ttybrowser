
static enum search_engine selected_search_engine;

static const char * const search_engines[] = {
  [GOOGLE] = "https://www.google.com/search?q=",
  [DUCKDUCKGO] = "https://duckduckgo.com/?q="
};

/*
 * Requests the given link
 *
 * returns the address of the response string from the server
 * (free needs to be done by the caller!)
 */
char * n_send_request(char *link) { }

/* 
 * Sends a request to the currently selected search engine
 * search_query will have it's spaces replaced by '+'
 * and will then be appended to the selected engines search string
 * 
 * returns the address of the response string from the server
 * (free needs to be done by the caller!)
 */
char * n_send_search_request(char *search_query) { }

/*
 * changes the used search engine
 */
void n_change_search_engine(enum search_type st) {
  selected_search_engine = st;
}
