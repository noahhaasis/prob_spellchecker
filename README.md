# Spellchecker
This is a spellchecker implementation which uses bloomfilters to query the dictionary.
Bloomfilters can save a lot of memory usage but membership queries can return a false positive.<br>
This is of course not the most ideal data structure for a spellchecker since it might mark correct words as misspelled but this is just a playground for experimenting with bloomfilters.

# Installation
`git clone https://github.com/noahhaasis/prob_spellchecker.git && cd prob_spellchecker && make`

# Usage
`./spellchecker <dictionary> <text>`
