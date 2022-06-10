## Shell
A simple shell. I used [this](https://brennan.io/2015/01/16/write-a-shell-in-c/) guide. Thank you [Stephen Brennan](https://github.com/brenns10) for the amazing guide!
My plan for this project is to add more features. My second goal is to get better at C, and to understand it better.

### Dependencies
I only use C standard libraries, except one. [GNU Readline](https://tiswww.case.edu/php/chet/readline/rltop.html). Make sure to install it before compiling. If you don't want to use GNU Readline, you can use the `old_input` branch instead.

### How to compile with GCC
`gcc shell.c config.c -o bin -lreadline -fcommon` Don't forget to add `-lreadline` and `-fcommon` when running the command.

### How to configure the prompt
You can easily configure the prompt. By default, the prompt configuration file is located at `/etc/prompt`. (This can be changed by adding the file location as a command line argument. Ex: `./shell /path/to/a/file`) 

Commands are separated by semicolons. Here is an example: `P;WS;$;WS`. This creates a prompt with the current path, a whitespace, $/#, and finally, another whitespace. If you enter a command that doesn't exist, it just prints it as it is.
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