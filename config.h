#ifndef CONFIG_H_
#define CONFIG_H_

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define BLD   "\x1B[1m"
#define ITC   "\x1B[3m"
#define RES   "\x1B[0m"

char **tokenize(char* prompt);
char *generate_prompt(char **tokens);
char *strremove(char *str, const char *sub);

#endif