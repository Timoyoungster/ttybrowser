
/*
 * array holding the links on the current page
 */
char **links;

/*
 * parses the input html to printable text
 * 
 * headings will be made bold
 * links will be made blue
 * 
 * (if possible javascript should be executed as well)
 */
char * p_get_printable_text(char * html);
