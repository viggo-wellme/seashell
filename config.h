#ifndef CONFIG_H_
#define CONFIG_H_

char **tokenize(char* prompt);
char *generate_prompt(char **tokens);
char *strremove(char *str, const char *sub);

#endif