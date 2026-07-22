# Game-of-Life
Usagi on YouTube (https://www.youtube.com/watch?v=bpdYBsgYF_c) gave me the idea to practice my C skills and play around with code for the Game of Life and different Cellular Automata. 

There are very good explanations on the internet about what the Game of Life is and how it relates to Cellular Automata, for example at https://jonmillen.com/1dlife/index.html. I started with the one dimensional version of it: the survival of a cell is dependent on the state of its direct neighbor cells to the left and to the right. For example, a simple rule is "An alive cell will survive into the next generation if it has 2 or 3 alive neighbors. A dead cell with three alive neighbors will be reborn". If alive cells are printed on the screen with a symbol or color and the dead cells are not visible anymore, the result are interesting patterns. Some look random, some rules create repeating patterns with a visible structure.

There is software out there that can do the same but in a prettier package, e.g. the Mathgrapher v2.03 (https://www.mathgrapher.com/cellular-automata-1d-rule/). My code has the advantage that it is short and very easy to understand. It does not even have a function outside of main().

To keep it simple, I created the code to print the patterns on the screen of an ANSI terminal. Green characters (cells) on a black background is a must. 

The best and most comprehensive book about this topic is Stephen Wolfram's "A New Kind of Science" (https://www.wolframscience.com/nks/p24--how-do-simple-programs-behave/). He documented all the different patterns and gave them a number. For example my code 1dlife-30.c is an implementation of his rule 30.

Alle files startung with "1dlife" are game of life variations in one dimension. The meaning of this is that decisions for the next cell generation are only done by taking the state of cells to the left and right into consideration. Files starting with "2dlife" are game of life variations in two dimensions. A number of random numbers of cells start in the middle of the screen. Decisions for the next generation of cells are done by looking at the state of all eight neighbor cells. 
