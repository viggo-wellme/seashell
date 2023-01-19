## Sea (C) Shell
A simple shell..

### Dependencies
I only use C standard libraries, except one. [GNU Readline](https://tiswww.case.edu/php/chet/readline/rltop.html). Make sure to install it before compiling. If you don't want to use GNU Readline, you can use the `old_input` branch instead.

### How to compile with GCC
`gcc src/shell.c src/config.c -o bin -lreadline` Don't forget `-lreadline`.

### How to run
Run it like any other program, just add a prompt config file location as an argument. Example: `./bin example_prompt`

### How to configure the prompt
You can easily configure the looks of your prompt by adding commands to your config file.

Commands are separated by semicolons. Here is an example: `P;WS;$;WS`. This creates a prompt with the current path, a whitespace, $/#, and finally, another whitespace. If you enter a command that doesn't exist, it just prints it as it is.

### List of commands:
|Command   |Action                                                        |
|----------|:-------------:                                               |
|`P`       |The current path                                              |
|`TP`      |The current path without but it replaces the home dir with `~`|
|`$`       |`#` if logged in as root, else `$`                            |
|`WS`      |Whitespace                                                    |

To add color to your prompt, use the following commands.
|Command   |Color                                                        
|----------|:-------------:                                               |
|`RED`     |Red|
|`GRN`     |Green|
|`YEL`     |Yellow|
|`BLU`     |Blue|
|`MAG`     |Magenta|
|`CYN`     |Cyan|
|`WHT`     |White|
|`BLD`     |Bold|
|`ITC`     |Italic|
|`RES`     |Reset colors.|

Thank you [Stephen Brennan](https://github.com/brenns10) for [this](https://brennan.io/2015/01/16/write-a-shell-in-c/) amazing guide!
