/* See LICENSE file for copyright and license details. */

#define STREMPTY(s) !s || strcmp(s, "") == 0

char *str_concat(char **s, int c);
char **str_split(const char *s, const char a_delim, int *c);
char *str_replace(char *orig, char *rep, char *with);
char *str_trim(char *s);
char *str_rmspaces(char *s);
int strinlist(char *str, char **list, int listc);
int charinstr(char c, char *str);
