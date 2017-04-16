/* See LICENSE file for copyright and license details. */
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

/* compress array of strings to single string */
char *
str_concat(char **s, int c)
{
	if (!s) die("strconcat: given null pointer");

	int len = 0, i;
	char *ret;

	for (i = 0; i < c; i++)
		len += strlen(s[i]) + 1;
	ret = ecalloc(len, sizeof(char));

	strcpy(ret, s[0]);
	strcat(ret, " ");
	for (i = 1; i < c; i++) {
		strcat(ret, s[i]);
		strcat(ret, " ");
	}

	return ret;
}

/* split string s into muiltple strings by a_delim */
char **
str_split(const char *s, const char a_delim, int *c)
{
	if (!s) die("strsplit: given null pointer");

	char **ret = 0;
	size_t count = 0;
	char *last_delim = 0;
	char delim[2] = { a_delim, 0 }; /* converet a_delim into string for strtok */
	char *a_str = ecalloc(strlen(s)+1, sizeof(char));
	strcpy(a_str, s);

	/* count number of elements that will be extracted. */
	for (char *tmp = a_str; *tmp; tmp++)
		if (a_delim == *tmp) {
			count++;
			last_delim = tmp;
		}

	/* add space for trailing token. */
	count += last_delim < (a_str + strlen(a_str) - 1);

	/* add space for terminating null string so caller
	 * knows where the list of returned strings ends. */
	count++;

	ret = ecalloc(count, sizeof(char*));

	if (ret) {
		size_t idx  = 0;
		char *token = strtok(a_str, delim);

		while (token) {
			assert(idx < count);
			*(ret + idx++) = estrdup(token);
			token = strtok(0, delim);
		}
		assert(idx == count - 1);
		*(ret + idx) = 0;
	}

	*c = count - 1;
	free(a_str);
	return ret;
}

/* replace rep in orig with with */
char *
str_replace(char *orig, char *rep, char *with)
{
	char *ret, *tmp, *ins; /* ins: the next insert point */
	int len_rep, len_with, len_front, count; /* len_front: distance between rep and end of last rep */

	/* checks, set lens, and init */
	if (!orig || !rep)
		return NULL;
	len_rep = strlen(rep);
	if (len_rep == 0)
		return NULL; /* empty rep causes infinite loop during count */
	if (!with)
		with = "";
	len_with = strlen(with);

	/* count the number of replacements needed */
	ins = orig;
	for (count = 0; (tmp = strstr(ins, rep)); ++count)
		ins = tmp + len_rep;

	tmp = ret = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

	if (!ret)
		return NULL;

	/* tmp: end of the ret string
	 * ins: next occurrence of rep in orig
	 * orig: remainder of orig after "end of rep" */
	while (count--) {
		ins = strstr(orig, rep);
		len_front = ins - orig;
		tmp = strncpy(tmp, orig, len_front) + len_front;
		tmp = strcpy(tmp, with) + len_with;
		orig += len_front + len_rep; /* move to next "end of rep" */
	}
	strcpy(tmp, orig);

	return ret;
}

/* remove tailing or leading white space from s */
char *
str_trim(char *s)
{
	char *end;

	/* trim leading space */
	while (isspace(*s)) s++;

	if (*s == 0) /* all spaces? */
		return s;

	/* trim trailing space */
	end = s + strlen(s) - 1;
	while (end > s && isspace(*end)) end--;

	/* write new null terminator */
	*(end+1) = 0;

	return s;
}

char *
str_rmspaces(char *s)
{
	int c = 0, d = 0;
	char *start = emalloc(strlen(s)+1);

	while (*(s+c) != '\0') {
		if (*(s+c) == ' ') {
			int temp = c + 1;
			if (*(s+temp) != '\0') {
				while (*(s+temp) == ' ' && *(s+temp) != '\0') {
					if (*(s+temp) == ' ') c++;
					temp++;
				}
			}
		}
		*(start+d) = *(s+c);
		c++;
		d++;
	}
	*(start+d)= '\0';

	return start;
}

/* return if str is in the list */
int
strinlist(char *str, char **list, int listc)
{
	if (!str || !list) return 0;
	int i;
	for (i = 0; i < listc; i++)
		if (list[i] && strcmp(str, list[i]) == 0)
			return 1;
	return 0;
}

int
charinstr(char c, char *str)
{
	if (!str) return 0;
	for (; *str; str++)
		if (*str == c)
			return 1;
	return 0;
}
