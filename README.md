# ttybrowser
## Underlying problem
I often would rather not have the browser running whilst coding.
*(battery saving, less distractions, minimalist feel, ...)*

But sometimes I just need a quick way to look something up on the
internet and so I end up starting the browser anyways.

## Solution
This project. It should be a quick way to run a google search and 
print out the results to the terminal in a human readable fashion.

## Usage
    ttybrowser [command] [argument]  
### Commands
    search [search_string]  (search for the given string; can be omitted)
    link   [link]    (requests the given link; if link is numeric it follows the link with that number from the previous page)
    engine [GOOGLE | DUCKDUCKGO]  (change the used search engine; *more may be added*)
