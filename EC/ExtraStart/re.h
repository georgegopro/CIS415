#ifndef _re_h_
#define _re_h_

typedef struct regexp RegExp;
typedef struct {
	int re_sindex;
	int re_eindex;
} RegExpMatch;

/*
 * re_create - create regular expression matching engine
 *             returns NULL if there is an error
 */
RegExp *re_create(void);

/*
 * re_compile - compile pattern to be used by the matching engine
 *              returns 1 if successful, 0 if error
 */
int re_compile(RegExp *re, char *pattern);

/*
 * re_status - return the status of the last operation on the engine
 *             as a 0-byte terminated string
 */
void re_status(RegExp *re, char *buf, int size);

/*
 * re_destroy - return any Heap storage associated with the engine
 */
void re_destroy(RegExp *re);

/*
 * re_match - check whether the supplied buffer matches the pattern
 *            return 1 if successful, 0 otherwise
 */
int re_match(RegExp *re, char *buf);

/*
 * re_regmatch - return information about matched sub-expressions
 *               returns number of matched sub-expressions as value of function
 */
int re_regmatch(RegExp *re, RegExpMatch **indices);

#endif /* _re_h_ */
