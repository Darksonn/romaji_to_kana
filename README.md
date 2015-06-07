# Type romaji and get kana

To start the program run the `./run` script in a terminal.  
Now simply start typing with roman letters, and it will be converted into kana when a complete kana has been written. Press `q` when you're done, and the program will exit and save what you've written in the clipboard.

Katakana is uppercase, hiragana is lowercase. Various japanese punctuation is also supported. Open the `gui.c` file and look in the init function to see possible completions.

If you want the result saved in a file instead of into the clipboard, run `make` to compile and use the `./rtok` executable with the filename as argument.

## Required software
 * gcc
 * xclip
 * ncurses
