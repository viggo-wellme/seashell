## Sea (C) Shell
Seashell is a minimal and simple shell.

### Dependencies
Seashell uses [GNU Readline](https://tiswww.case.edu/php/chet/readline/rltop.html), so you will have to install that before compiling. It does not use any other dependency except for the default C libraries. If you want Seashell to be as minimal as possible, you could write your own readline function using something like `getchar()`. This is what I did before I started using GNU Readline for this project.

### How to compile
Any modern C compiler will probably work for this, but I use GCC. To compile, use the following command: `gcc src/shell.c src/config.c -o bin -lreadline` Don't forget `-lreadline`.

### How to run
Seashell does not have any command line arguments, so you should run it as you would with any other program.

### Configuration
Configuring Seashell is done in the `config.c` and `config.h` file. It is similar to how you configure programs such as dwm, st, etc. Remember that you will need to recompile the program after changing these files.
#### `config.h`
config.h contains the definition of `gen_prompt()` which is used in `shell.c`. It also contains definitions of some ANSI escape codes which will be very useful for when configuring the prompt. You will most likely only need to edit this file when adding, removing, or editing the ANSI escape codes.
#### `config.c`
This is the main file for configuring the prompt. For most users, you will only need to edit the first function, `gen_prompt()`. This function simply returns the text that is prompted to the user, for example `user@pc /home/user >`. The different "components" of the prompt are simply added to the variable named `prompt` using `strcat`. To create a prompt like the example I showed you, the code would look something like this:
```c
strcat(prompt, get_username());
strcat(prompt, "@");
strcat(prompt, get_hostname());
strcat(prompt, " ");
strcat(prompt, pwd());
strcat(prompt, " > ");
```
Note: If your prompt is large, you might need to change the `PROMPT_SIZE` macro.

There are also other functions in this file, for example `get_username` and `pwd`. The purpose of these functions are to make it easier for you to create your own prompt. You can of course edit or delete these and you can even add your own functions.

#### `colors and text decorations`
To add colors and text decorations we use ANSI escape codes. There are a few codes already defined for you in `config.h`. Example:
```c
// config.h
#define RED "\x1B[31m"
// config.c
strcat(prompt, RED);
```
You can also use your own ANSI escape codes like this:
```c
// Set style to bold, red foreground
// config.c
strcat(prompt, "\x1b[1;31m");
// you can also create escape codes macros like this to make your config easier to read:
// config.h
#define BOLD_RED_F "\x1b[1;31m"
```