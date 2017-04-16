/* See LICENSE file for copyright and license details. */

#define MAX(A, B)         ((A) > (B) ? (A) : (B))
#define MIN(A, B)         ((A) < (B) ? (A) : (B))
#define BETWEEN(X, A, B)  ((A) <= (X) && (X) <= (B))
#define LEN(x)            ((int)(sizeof (x) / sizeof *(x)))

#define DUMPD(x) printf("%s: %d\n", #x, x)
#define DUMPS(x) printf("%s: %s\n", #x, x)

void *ecalloc(size_t nmemb, size_t size);
void *emalloc(size_t size);
void *erealloc(void *p, size_t size);
char *estrdup(char *s);

void die(const char *errstr, ...);
