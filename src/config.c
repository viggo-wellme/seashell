/* config.c -- The config file */

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
#include <unistd.h>
#include "config.h"
#include <unistd.h>
#include <linux/limits.h>
#include <bits/local_lim.h>

char *get_hostname();
char *pwd();
const char *get_username();

// Returns the prompt
#define PROMPT_SIZE 200
char *gen_prompt() {
    char *prompt = malloc(PROMPT_SIZE);
    prompt[0] = '\0';

    // Add hostname to prompt
    strcat(prompt, get_username());
    strcat(prompt, "@");
    strcat(prompt, get_hostname());
    strcat(prompt, " ");
    strcat(prompt, pwd());
    strcat(prompt, " > ");
    

    return prompt;
}

#define DEF_HOSTNAME_SIZE 10
char *get_hostname() {
    int hostname_bytes = DEF_HOSTNAME_SIZE;
    char *hostname = malloc(hostname_bytes);

    while (gethostname(hostname, hostname_bytes) == -1) {
        hostname_bytes++;
    }
    return hostname;
}

// Returns the current path
#define PWD_SIZE 100
char *pwd() {
    char *cwd = malloc(PWD_SIZE);
    if (getcwd(cwd, PWD_SIZE) == NULL) {
        printf("error at: %s, func: %s, line: %d\n", __FILE__, __FUNCTION__, __LINE__);
        perror("seashell");
    }
    return cwd;
}

// Get the current username
const char *get_username() {
    const char *usr_env_var = getenv("USER");
    if (usr_env_var != NULL) {
        return getenv("USER");
    } else {
        printf("error at: %s, func: %s, line: %d\n", __FILE__, __FUNCTION__, __LINE__);
        perror("seashell");        
    }
}