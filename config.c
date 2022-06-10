#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include <unistd.h>
#include <linux/limits.h>

char **tokenize(char *prompt);
char *strremove(char *str, const char *sub);
char *generate_prompt(char **tokens);
char *_prompt();

char **tokenize(char* prompt) {
    // Takes a string and splits it into tokens
    char *token = strtok(prompt, ";");
    char **tokens = malloc(sizeof(char)*500);
    int pos = 0;
    while( token != NULL ) {
        tokens[pos] = token;
        pos++;
        token = strtok(NULL, ";");    
    }
    return tokens;
}

char *generate_prompt(char **tokens) {
    char *buffer = malloc(200);
    int i;
    char $[2] = "#\0";
    char cwd[PATH_MAX];

    // If not root
    if (geteuid() != 0) {
        $[0] = '$';
        $[1] = '\0';
    }
    for (i = 0; i < 50; i++) {
        if (tokens[i] != NULL) {
            if (tokens[i][0] == 'P') {
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    strcat(buffer, cwd);
                }
            }
            else if (tokens[i][0] == 'T' && tokens[i][1] == 'P') {
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    if (strstr(cwd, getenv("HOME"))) {
                        strremove(cwd, getenv("HOME"));
                        strcat(buffer, "~");
                    }
                    strcat(buffer, cwd);
                }
            }            
            else if (tokens[i][0] == 'W' && tokens[i][1] == 'S') {
                strcat(buffer, " ");
            }
            else if (tokens[i][0] == '$') {
                strcat(buffer, $);
            }
            else if (tokens[i][0] == 'R' && tokens[i][1] == 'E' && tokens[i][2] == 'D') {
                strcat(buffer, RED);
            }
            else if (tokens[i][0] == 'G' && tokens[i][1] == 'R' && tokens[i][2] == 'N') {
                strcat(buffer, GRN);
            }
            else if (tokens[i][0] == 'Y' && tokens[i][1] == 'E' && tokens[i][2] == 'L') {
                strcat(buffer, YEL);
            }
            else if (tokens[i][0] == 'B' && tokens[i][1] == 'L' && tokens[i][2] == 'U') {
                strcat(buffer, BLU);
            }
            else if (tokens[i][0] == 'M' && tokens[i][1] == 'A' && tokens[i][2] == 'G') {
                strcat(buffer, MAG);
            }
            else if (tokens[i][0] == 'C' && tokens[i][1] == 'Y' && tokens[i][2] == 'N') {
                strcat(buffer, CYN);
            }
            else if (tokens[i][0] == 'W' && tokens[i][1] == 'H' && tokens[i][2] == 'T') {
                strcat(buffer, WHT);
            }
            else if (tokens[i][0] == 'R' && tokens[i][1] == 'E' && tokens[i][2] == 'S') {
                strcat(buffer, RES);
            }
            else if (tokens[i][0] == 'B' && tokens[i][1] == 'L' && tokens[i][2] == 'D') {
                strcat(buffer, BLD);
            }
            else if (tokens[i][0] == 'I' && tokens[i][1] == 'T' && tokens[i][2] == 'C') {
                strcat(buffer, ITC);
            }
            else {
                strcat(buffer, tokens[i]);
            }
        }
    }
    // Clear cwd
    memset(cwd, 0, strlen(cwd));
    return buffer;
}

char *strremove(char *str, const char *sub) {
    // Length of substring
    size_t len = strlen(sub);
    // If the length is not 0
    if (len > 0) {
        char *p = str;
        // While substring is in string
        while ((p = strstr(p, sub)) != NULL) {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}