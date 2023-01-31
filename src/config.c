/* config.c -- Parses configuration files. */

/*
    Copyright (C) 2022 Viggo Wellme

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
#include "config.h"
#include <unistd.h>
#include <linux/limits.h>
#include <bits/local_lim.h>

char **tokenize(char* prompt) {
    // Takes a string and splits it into tokens
    char *token = strtok(prompt, ";");
    char **tokens = malloc(sizeof(char)*1000);
    int pos = 0;
    while(token != NULL) {
        tokens[pos] = token;
        pos++;
        token = strtok(NULL, ";");    
    }
    return tokens;
}

char *generate_prompt(char **tokens) {
    char *buffer = malloc(200);
    char $[2] = "#\0";
    char cwd[PATH_MAX];
    char hostname[HOST_NAME_MAX + 1];

    // If not root
    if (geteuid() != 0) {
        $[0] = '$';
        $[1] = '\0';
    }

    
    gethostname(hostname, HOST_NAME_MAX + 1);

    for (int i = 0; i < 50; i++) {
        if (tokens[i] != NULL) {
            if (strcmp(tokens[i], "P") == 0) {
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    strcat(buffer, cwd);
                }
            }
            else if (strcmp(tokens[i], "TP") == 0) {
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    if (strstr(cwd, getenv("HOME"))) {
                        strremove(cwd, getenv("HOME"));
                        strcat(buffer, "~");
                    }
                    strcat(buffer, cwd);
                }
            }
            else if (strcmp(tokens[i], "WS") == 0) {
                strcat(buffer, " ");
            }
            else if (strcmp(tokens[i], "$") == 0) {
                strcat(buffer, $);
            }
            else if (strcmp(tokens[i], "USR") == 0) {
                strcat(buffer, getlogin());
            }
            else if (strcmp(tokens[i], "HOST") == 0) {
              	strcat(buffer, hostname);
	    }
            else if (strcmp(tokens[i], "RED") == 0) {
                strcat(buffer, RED);
            }
            else if (strcmp(tokens[i], "GRN") == 0) {
                strcat(buffer, GRN);
            }
            else if (strcmp(tokens[i], "YEL") == 0) {
                strcat(buffer, YEL);
            }
            else if (strcmp(tokens[i], "BLU") == 0) {
                strcat(buffer, BLU);
            }
            else if (strcmp(tokens[i], "MAG") == 0) {
                strcat(buffer, MAG);
            }
            else if (strcmp(tokens[i], "CYN") == 0) {
                strcat(buffer, CYN);
            }
            else if (strcmp(tokens[i], "WHT") == 0) {
                strcat(buffer, WHT);
            }
            else if (strcmp(tokens[i], "RES") == 0) {
                strcat(buffer, RES);
            }
            else if (strcmp(tokens[i], "BLD") == 0) {
                strcat(buffer, BLD);
            }
            else if (strcmp(tokens[i], "ITC") == 0) {
                strcat(buffer, ITC);
            }
            else {
                strcat(buffer, tokens[i]);
            }
        }
    }
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

char *home_dir_replace(char *line) {
    int i;
    int line_size = strlen(line);
    char *buffer = malloc(200);
    
    for (i=0; i < line_size; i++) {
        if (line[i] == '~') {
            strcat(buffer, getenv("HOME"));
        }
        else {
            char tmp[2];
            tmp[0] = line[i];
            tmp[1] = '\0';
            strcat(buffer, tmp);
        }
    }
    return buffer;
}
