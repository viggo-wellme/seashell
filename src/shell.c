/* shell.c -- The main file. */

/*
    Copyright (C) 2022  Viggo Wellme

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <linux/limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include "config.h"

#define NAME "seashell"
#define VERSION "1"

#define ERR_ALLOC_ER "allocation error\n"
#define S_TOK_DELIM " \t\r\n\a"

#define RL_BUFSIZE 1024
#define TOK_BUFSIZE 64
#define GH_BUFSIZE 64
#define REPL_BUFSIZE 128

char **split_line(char *line);
void main_loop();

int launch(char **args);
int execute(char **args);
int launch(char **args);

int num_builtins();
int builtin_cd(char **args);
int builtin_help(char **args);
int builtin_exit(char **args);
void ctrl_c_handler(int signal);
int file_exists(char *file);

char *prompt;
// List of the names of builtin commands
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

// List of the builtin functions
int(*builtin_func[]) (char **) = {
    &builtin_cd,
    &builtin_help,
    &builtin_exit
};

int main(int argc, char *argv []) {
    main_loop();
    return 0;
}

void main_loop() {
    char *line;
    char **args;
    int status;
    do {
        line = readline(gen_prompt());
        if (line && *line) {
            // Add to command history if a command was entered
            add_history(line);
        }
        //line = home_dir_replace(line);
        args = split_line(line);
        status = execute(args);
        free(line);
        free(args);
    } while (status);
}


char **split_line(char *line) {
    // Splits a line into an array of tokens. "one two three" -> ["one", "two", "three"]
    int bufsize = TOK_BUFSIZE;
    int position = 0;
    // The array that will be returned
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;
    
    if (!tokens) {
        // If failed to allocate memory with malloc
        fprintf(stderr, ERR_ALLOC_ER);
        exit(EXIT_FAILURE);
    }
    // Split the line
    token = strtok(line, S_TOK_DELIM);
    while (token != NULL) {
        // Add the token to the tokens array
        tokens[position] = token;
        position++;

        // If the buffer size is reached
        if (position >= bufsize) {
            // Expand the buffer sizes
            bufsize += TOK_BUFSIZE;
            // Reallocate the array
            tokens = realloc(tokens, bufsize * sizeof(char*));
            // Check for allocation errors
            if (!tokens) {
                fprintf(stderr, ERR_ALLOC_ER);
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, S_TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

int builtin_cd(char **args) {
    if (args[1] == NULL) {}
    else if (chdir(args[1]) != 0) {
        perror(NAME);
    }
    return 1;
}

int builtin_help(char **args) {
    int i;
    printf("%s%s%s%s", NAME, " version ", VERSION, "\nBuiltin commands:\n");
    printf(" ´help´: shows this message\n");
    printf(" ´cd´: changes the directory. Example: ´cd dir´\n");
    printf(" ´exit´: exits the shell\n");
    return 1;
}

int builtin_exit(char **args) {
    return 0;
}

int num_builtins() {
    // Returns the amount of builtin commands
    return sizeof(builtin_str) / sizeof(char *);
}

int execute(char **args) {
    int i;
    if (args[0] == NULL) {
        // Return 2 if command is empty
        return 2;
    }
    
    for (i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            // Strcmp returns 0 if 2 strings match
            // So if first arg matches a builtin
            // Call the builtin with args
            return (*builtin_func[i])(args);
        }
    }
    // Run a regular process
    return launch(args);
}

int launch(char **args) {
    pid_t pid; 
    pid_t wpid;
    int status; 
    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            // If command not found
            printf("%s%s%s\n", NAME, ": command not found: ", args[0]);
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0) {
        // If fork() had an error
        perror(NAME);
    }
    else {
      // Parent process
      do {
          wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
    return 1;
}

void ctrl_c_handler(int _signal) {
    // Just print the prompt if ctrl c was clicked
    printf("%s%s\n%s", NAME, ": to exit, use the `exit` command.", prompt);
}

int file_exists(char *file) {
    if (access(file, F_OK ) == 0) {
        return 1;
    } else {
        return 0;
    }    
}
