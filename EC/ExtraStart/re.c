#include "re.h"
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

#define ALLOCATED 1
#define COMPILED 2
#define NPATTERNS 50

struct regexp {
	regex_t preg;
	int status;
	int compSts;
	int execSts;
	int nMatch;
	RegExpMatch pMatch[NPATTERNS];
};

/*
 * re_create - create regular expression matching engine
 *             returns NULL if there is an error
 */
RegExp *re_create(void) {
	RegExp *re = (RegExp *)malloc(sizeof(RegExp));
	if (re != NULL) {
		re->status = ALLOCATED;
		re->compSts = 0;
		re->nMatch = 0;
	}
	return re;
}

/*
 * re_compile - compile pattern to be used by the matching engine
 *              returns 1 if successful, 0 if error
 */
int re_compile(RegExp *re, char *pattern) {
	re->execSts = 0;
	if (re->status == COMPILED) {
		regfree(&(re->preg));
		re->status = ALLOCATED;
	}
	re->compSts = regcomp(&(re->preg), pattern, REG_EXTENDED|REG_NEWLINE);
	if (re->compSts == 0) {
		re->status = COMPILED;
	}
	return (re->compSts == 0);
}

/*
 * re_status - return the status of the last operation on the engine
 *             as a 0-byte terminated string
 */
void re_status(RegExp *re, char *buf, int size) {
	if (re->compSts)
		(void) regerror(re->compSts, &(re->preg), buf, size);
	else if (re->execSts && re->execSts != REG_NOMATCH)
		(void) regerror(re->execSts, &(re->preg), buf, size);
	else
		buf[0] = '\0';
}

/*
 * re_destroy - return any Heap storage associated with the engine
 */
void re_destroy(RegExp *re) {
	if (re->status == COMPILED)
		regfree(&(re->preg));
	free(re);
}

/*
 * re_match - check whether the supplied buffer matches the pattern
 *            return 1 if successful, 0 otherwise
 */
int re_match(RegExp *re, char *buf) {
	size_t nmatch = NPATTERNS;
	regmatch_t pmatch[NPATTERNS];

	if (re->status == COMPILED) {
		int n, i;
		re->execSts = regexec(&(re->preg), buf, nmatch, pmatch, 0);
		if (re->execSts != 0)
			return 0;
		for (n = 0; n < NPATTERNS; n++)
			if (pmatch[n].rm_so == -1)
				break;
		re->nMatch = n;
		for (i = 0; i < n; i++) {
			re->pMatch[i].re_sindex = pmatch[i].rm_so;
			re->pMatch[i].re_eindex = pmatch[i].rm_eo;
		}
		return 1;
	}
	return 0;
}

/*
 * re_regmatch - returns information about matched sub-expressions
 *               returns number of matched sub-expressions as value of function
 */
int re_regmatch(RegExp *re, RegExpMatch **indices) {
	*indices = re->pMatch;
	return re->nMatch;
}
