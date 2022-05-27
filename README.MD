## Shell
A simple shell. I used [this](https://brennan.io/2015/01/16/write-a-shell-in-c/) guide. Thank you [Stephen Brennan](https://github.com/brenns10) for the amazing guide!
My plan for this project is to add more features. My second goal is to get better at C, and to understand it better.

### Dependencies
I only use C standard libraries, except one. [GNU readline](https://tiswww.case.edu/php/chet/readline/rltop.html). Make sure to install it before compiling.

### How to compile with GCC
`gcc shell.c -o bin -lreadline` Don't forget to add `-lreadline` when running the command.