# Automata

This is a basic automation program assigned by NYU as a C programming assignment. 

### Description

The automation program that we were asked to program is also known as “The Game of Life” which is a cellular automation devised by the 
british mathematician John Horton Conway (Some article’s refer it as Conway’s Game of life, see: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life )

I was faced with this task before in: https://leetcode.com/problems/game-of-life/ . Therefore, I was able to recall the solution at first sight. 
However, the challenge for me was to create a solution using the C language. My previous endeavors were with the Python programming language.

First I had to practice File I/O. Which was not a daunting task. C standard provides convenient ways to deal with files. 

Second step was to prepare the solution. The solution could be programmed in multiple different styles. Constraints often play a significant 
role in constructing a solution. However, the assignment 02 did not have any constraints. I had two solutions in my mind. First was to solve 
this problem recursively. A python way to solve this problem would be to keep a set. Recursively iterate over each cell, add each visited cell 
to the set(to prevent infinite loops), count the live neighbors before doing a recursive call and update the cell before returning. However, the 
C standard does not provide data structures, and creating hash_map is a daunting task. Fortunately assignment 02 notes the size of the grid to be 5x5.
This enables the use of an additional 5x5 grid to keep track of visited cells. 

However, this solution would only make sense if assignment 02 had a constraint to update the grid in place. Since there is no such constraint, 
a better solution would be to use two grids. One for the current generation and other for the next generation. 
After updating each cell; copying the next generation to the current generation, and repeating this task the number of times the user requests.
Both solutions will have the same time complexity: t(5*5) = 25t (t being user’s input). However since recursion takes space in the stack frame, 
the second solution would have a slight advantage regarding the space complexity. Hence the second solution is what I programmed.
