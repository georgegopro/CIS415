#include "mlist.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct mlistnode {
	struct mlistnode *next;
	MEntry *entry;
} MListNode;

struct mlist {
	struct mlistnode *first;
	struct mlistnode *last;
};

/* ml_create - created a new mailing list */
MList *ml_create(void) {
	MList *p;

	if ((p = (MList *)malloc(sizeof(MList))) != NULL) {
		p->first = NULL;
		p->last = NULL;
	}
	return p;
}

/* ml_add - adds a new MEntry to the list;
 * returns 1 if successful, 0 if error (malloc)
 * returns 1 if it is a duplicate */
int ml_add(MList **ml, MEntry *me) {
	MList *p;
	MListNode *q;

	p = *ml;
	if (ml_lookup(p, me) != NULL)
		return 1;
	if ((q = (MListNode *)malloc(sizeof(MListNode))) == NULL)
		return 0;
	q->entry = me;
	q->next = p->first;
	p->first = q;
	if (!(p->last))
		p->last = q;
	return 1;
}

/* ml_lookup - looks for MEntry in the list, returns matching entry or NULL */
MEntry *ml_lookup(MList *ml, MEntry *me) {
	MList *p;
	MListNode *q;

	p = ml;
	for (q = p->first; q != NULL; q = q->next)
		if (me_compare(me, q->entry) == 0)
			return q->entry;
	return NULL;
}

/* ml_destroy - destroy the mailing list */
void ml_destroy(MList *ml) {
	MListNode *q;

	q = ml->first;
	while (q != NULL) {
		MListNode *r = q->next;
		me_destroy(q->entry);
		free(q);
		q = r;
	}
	free(ml);
}
