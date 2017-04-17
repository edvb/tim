/* See LICENSE file for copyright and license details. */
#include <ctype.h>
#include <limits.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arg.h"
#include "str.h"
#include "tim-lua.h"
#include "util.h"

/* macros */
#define MAX_CMD 2048

/* typedefs */

/* functions */
char *get_tim_ans(char *s);

char *fmt_cmd(char *s);
void run_cmd(void);
void print_out(void);

void setup(void);
void cleanup(void);
void usage(void);

/* variables */
char *argv0;

char *in;
char *cmd;
char *out;
int fmtout = 1;

#include "config.h"

/* returns the path to fname based on:
 *  1) $XDG_CONFIG_HOME/tim/ (~/.config/tim/)
 *  3) /etc/tim/             (system wide config)
 *  2) /usr/local/share/tim/ (default when installed)
 *  4) ./                    (current directory)
 */
char *
get_path(const char *fname)
{
	if (!fname) return NULL;
	int i;
	static char ret[PATH_MAX];
	const char *paths[] = { "/etc/tim/", "/usr/local/share/tim/", "./" };
	const char *home = getenv("HOME");
	const char *xdg_config = getenv("XDG_CONFIG_HOME");
	struct passwd *pw;

#ifdef DEBUG
	/* if DEBUG is set check current directory first */
	strcpy(ret, "./");
	strcat(ret, fname);
	if (access(ret, F_OK) != -1)
		return ret;
#endif

	/* try to get users home directory */
	if (!home || !*home)
		if ((pw = getpwuid(getuid())))
			home = pw->pw_dir;

	/* get XDG_CONFIG_HOME path */
	if (xdg_config)
		snprintf(ret, sizeof(ret), "%s/tim/%s", xdg_config, fname);
	else if (home && *home)
		snprintf(ret, sizeof(ret), "%s/.config/tim/%s", home, fname);

	/* try rest of paths in paths[] */
	for (i = 0; access(ret, F_OK) == -1; i++) {
		if (i >= LEN(paths)) die("%s: file '%s' not found", argv0, fname);
		strcpy(ret, paths[i]);
		strcat(ret, fname);
	}

	return ret;
}

char *
get_tim_ans(char *s)
{
	if (!s) return NULL;
	char *ans = NULL;
	int c, i = 2;
	char **cmds;
	char *fname = NULL;
	char args[MAX_CMD];

	s = str_replace(s, "to", "");
	char *tmp = str_rmspaces(s);
	cmds = str_split(tmp, ' ', &c);

	if (c > 2) {
		if (strcmp(cmds[1], "my") == 0)
			fname = get_path(memfile);
		else if (strcmp(cmds[1], "your") == 0)
			fname = get_path(cogfile);
		else {
			fname = get_path(cogfile);
			i--;
		}
	}

	if (strcmp(cmds[0], "get") == 0 && c > 2) {
		snprintf(args, sizeof(args), "%s %s", fname, cmds[i]);
		ans = tim_lua_run(get_path("plugins/json-get.lua"), args);
	} else if (strcmp(cmds[0], "set") == 0 && c > 3) {
		snprintf(args, sizeof(args), "%s %s %s", fname, cmds[i], cmds[i+1]);
		ans = tim_lua_run(get_path("plugins/json-set.lua"), args);
	}

	free(s);
	free(tmp);
	for (int j = 0; j < c; j++)
		free(cmds[j]);
	free(cmds);

	return ans;
}

char *
fmt_cmd(char *s)
{
	int i;
	for (i = 0; s[i]; i++)
		s[i] = tolower(s[i]); /* TODO not for proper nouns */
	return s;
}

/* generate tim's response */
void
run_cmd(void)
{
	char args[MAX_CMD];
	cmd = fmt_cmd(in);
	snprintf(args, sizeof(args), "%s %s", get_path(actfile), cmd);
	out = tim_lua_run(get_path("plugins/json-get.lua"), args);
	if (STREMPTY(out))
		out = get_tim_ans(cmd);
	if (STREMPTY(out))
		out = tim_lua_run(get_path("plugins/weather.lua"), cmd);
	if (STREMPTY(out))
		out = tim_lua_run(get_path("plugins/rand.lua"), cmd);
	if (STREMPTY(out)) {
		out = tim_lua_run(get_path("plugins/ddg.lua"), in);
		fmtout = 0;
	}
	if (STREMPTY(out))
		out = estrdup("Sorry I didn't get that");
}

/* print tim's respond */
void
print_out(void)
{
	if (!out) return;
	printf("%s\n", out);
	out = NULL;
}

/* allocate strs */
void
setup(void)
{
	in  = ecalloc(MAX_CMD, sizeof(char));
	out = ecalloc(MAX_CMD, sizeof(char));
}

/* free memory */
void
cleanup(void)
{
	free(in);
	free(out);
}

void
usage(void)
{
	die("usage: %s [-iv] [CMD]", argv0);
}

int
main(int argc, char *argv[])
{
	ARGBEGIN {
		case 'i':
			interactive = 1;
			break;
		case 'v':
			printf("%s v%s\n", argv0, VERSION);
			return 0;
		default:
			usage();
	} ARGEND;

	setup();

	do {
		if (argc > 0) {
			in = str_concat(argv, argc);
			argc = 0;
		} else
			fgets(in, MAX_CMD, stdin);
		str_trim(in);
		run_cmd();
		print_out();
	} while (interactive && !feof(stdin));

	cleanup();

	return 0;
}
