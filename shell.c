#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>

#define ERR_ALLOC_ER "allocation error\n"
#define RL_BUFSIZE 1024
#define TOK_BUFSIZE 64
#define GH_BUFSIZE 64
#define REPL_BUFSIZE 128
#define TOK_DELIM " \t\r\n\a"
#define NAME "shell"
#define VERSION "1"

#define left() printf("\033[1D");
#define right() printf("\033[1C");

char *read_line(void);
char **split_line(char *line);
void main_loop(void);

int launch(char **args);
int execute(char **args);
int launch(char **args);

int num_builtins();
int builtin_cd(char **args);
int builtin_help(char **args);
int builtin_exit(char **args);
char *home_dir();
char *replace_str(char* str, char* replace, char* replace_with);
int str_size(char* string);

// List of the names of builtin commands
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

// List of the builtin functions
int (*builtin_func[]) (char **) = {
  &builtin_cd,
  &builtin_help,
  &builtin_exit
};

int main(int argc, char *argv []) {
    main_loop();
    return 0;
}

void main_loop(void) {
    char *line;
    char **args;
    int status;
    do {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s%s", cwd, " > ");
        }
        line = read_line();
        args = split_line(line);
        status = execute(args);
        free(line);
        free(args);
    } while (status);
}

char *read_line(void) {
    int bufsize = RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    // The reason c in of tye int is because you can check end of file. You cannot do that with char
    int c;

    if (!buffer) {
        // If failed to allocate memory with malloc
        fprintf(stderr, ERR_ALLOC_ER);
        exit(EXIT_FAILURE);
    }   
    while (1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            // If we are at end of file or newline; add null char (\0) and return the string
            buffer[position] = '\0';
            return buffer;
        }
        else {
            // Else; add the char to the buffer
            buffer[position] = c;
        }
        position++;

        // If the buffer size is reached
        if (position >= bufsize) {
            // Make the buffer size bigger
            bufsize += RL_BUFSIZE;
            // Reallocate the buffer with the new buffer size
            buffer = realloc(buffer, bufsize);
            // Check if the reallocation was successful
            if (!buffer) {
                fprintf(stderr, ERR_ALLOC_ER);
                exit(EXIT_FAILURE);
            }
        }
    }
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
    token = strtok(line, TOK_DELIM);
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

        token = strtok(NULL, TOK_DELIM);
    }
    // Null terminate
    tokens[position] = NULL;
    return tokens;
}

int builtin_cd(char **args) {
    if (args[1] == NULL) {
        // If no argument after cd
        char err[50] = NAME;
        strcat(err, ": expected argument to \"cd\"\n");
        fprintf(stderr, err);
    }

    else {
        if (chdir(args[1]) != 0) {
            perror(NAME);
        }
    }
    return 1;
}

int builtin_help(char **args) {
    printf(NAME);
    printf(" version ");
    printf(VERSION);
    return 1;
}

int builtin_exit(char **args) {
    return 0;
}

int num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int execute(char **args) {
    int i;
    if (args[0] == NULL) {
        // If command is empty
        return 1;
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
            // If execvp returns -1; print error
            perror(NAME);
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

int str_size(char* string) {
    int i = 0;
    while(string[i] != '\0')
    {
        i++;
    }
    return i;
}

char *home_dir() {
    char *path = malloc(sizeof(char) * GH_BUFSIZE);
    // If HOME does not exist
    if(!getenv("HOME")) {
        return NULL;
    }
    // If the buffersize is too small
    if(snprintf(path, GH_BUFSIZE, "%s", getenv("HOME")) >= GH_BUFSIZE){
        return NULL;
    }
    return path;
}

char *replace_str(char* str, char* replace, char* replace_with) {
    int strlen = str_size(replace_with);
    char * pch;
    pch = strstr (str, replace);
    if (pch != NULL) {
        strncpy(pch, replace_with, strlen);
    }
    return str;
}