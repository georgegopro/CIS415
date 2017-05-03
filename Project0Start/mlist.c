/*
 * Benjamin Barnes
 * 951289352
 * CIS415 Project 0
 *
 * This is my own work
 */

#include <stdio.h>
#include <stdlib.h>
#include "mentry.h"
#include "mlist.h"

// you can change how much the hash map scales by
#define SCALEFACTOR 2

/* node struct of the 'buckets' */
typedef struct mlistnode {
    struct mlistnode *next;
    MEntry *entry;
} Node;

/* struct used to hold pointers to 'buckets' */
typedef struct llist{
    int size;
    Node *head;
} LList;

/* struct used as the mailing list */
struct mlist{
    unsigned long size;
    LList **map;
};

MList *resize(MList *ml, int scale);

/* ml_create - created a new mailing list */
MList *ml_create(void){
    int i;
    MList *mailing_list;

    if((mailing_list = (MList *)malloc(sizeof(MList))) == NULL) return mailing_list;

    mailing_list->size = 100;

    if((mailing_list->map = (LList **)calloc(mailing_list->size, sizeof(LList *))) == NULL) return mailing_list;

    for(i = 0; i < mailing_list->size; i++){
        if((mailing_list->map[i] = (LList *)malloc(sizeof(LList))) == NULL) return mailing_list;
        mailing_list->map[i]->head = (Node *)NULL;
    }
    return mailing_list;

}


/* transfers entries in ml1 to ml2 */
void transfer(MList *ml1, MList *ml2){
    int i;
    Node *current;
    for(i = 0; i < ml1->size; i++){
        current = ml1->map[i]->head;
        while(current != NULL){
            ml_add(&ml2, current->entry);
            current = current->next;
        }
    }
}


/* used to destory MList structs
 * we have this in addtion to ml_destory
 * because this one does not destroy entries,
 * rather just pointers to entries from a struct no
 * longer being used*/
void ml_destroy_old(MList *old){
    int i;
    Node *temp;

    for(i = 0; i < old->size; i++){
        temp = old->map[i]->head;
        while(temp != NULL){
            Node *r = temp->next;
            free(temp);
            temp = r;
        }
        free(old->map[i]);
    }
    free(old->map);
}


/* ml_add - adds a new MEntry to the list;
 * returns 1 if successful, 0 if error (malloc)
 * returns 1 if it is a duplicate */
int ml_add(MList **ml, MEntry *me){
    MList *mailing_list, *new_mailing_list;
    Node *new;
    LList *correct_bucket;
    unsigned long value;

    mailing_list = *ml;

    if (ml_lookup(mailing_list, me) != NULL) return 1;

    // if we get here we know we need to add it
    if ((new = (Node *)malloc(sizeof(Node))) == NULL) return 0;
    new->next = NULL;
    new->entry = me;

    // get hash value
    value = me_hash(me, mailing_list->size);

    // get correct bucket
    correct_bucket = mailing_list->map[value];

    // see if its head is null, aka there is nothing in it
    if(correct_bucket->head == NULL){
        correct_bucket->head = new;
        correct_bucket->size = 1;
        return 1;
    }

    // bucket will be equal to size 20 or less after node addition; we dont resize
    if(correct_bucket->size <= 19){
        new->next = correct_bucket->head;
        correct_bucket->head = new;
        correct_bucket->size++;
        return 1;
    }

    // size is 20, so adding an element will make it too big
    // we add the element and then resize it
    if(correct_bucket->size >= 20){
        // temporarily add entry to bucket - it is about to be resized
        new->next = correct_bucket->head;
        correct_bucket->head = new;
        correct_bucket->size++;

        // make new list "the main list"
        new_mailing_list = resize(mailing_list, SCALEFACTOR);

        // transfer old mailing entries to new
        transfer(*ml, new_mailing_list);
        // destroy everything in old mailing list except actual entries
        ml_destroy_old(*ml);
        // set new mailing list to the new hashmap
        **ml = *new_mailing_list;
        free(new_mailing_list);
    }
    return 1;
}


/* resizes the given mailing list */
MList *resize(MList *ml, int scale){
    fprintf(stderr, "Resizing\n");
    int i;
    unsigned long new_size;
    MList *resized_ml;

    // calculate new size
    new_size = ml->size * scale;

    if((resized_ml = (MList *)malloc(sizeof(MList))) == NULL) return resized_ml;

    resized_ml->size = new_size;

    if((resized_ml->map = (LList **)calloc(resized_ml->size, sizeof(LList *))) == NULL) return resized_ml;

    // allocating memory for each LList and setting head node to null
    for(i = 0; i < resized_ml->size; i++){
        if((resized_ml->map[i] = (LList *)malloc(sizeof(LList))) == NULL) return resized_ml;
        resized_ml->map[i]->head = (Node *)NULL;
    }

    return resized_ml;

}


/* ml_lookup - looks for MEntry in the list, returns matching entry or NULL */
MEntry *ml_lookup(MList *ml, MEntry *me){
    int i;
    unsigned long value;
    MList *mailing_list;
    Node *current;

    mailing_list = ml;
    value = me_hash(me, mailing_list->size);

    current = mailing_list->map[value]->head;

    while(current != NULL){
        if(me_compare(me, current->entry) == 0)
            return current->entry;
        current = current->next;
    }
    return NULL;
}


/* ml_destroy - destroy the mailing list */
void ml_destroy(MList *ml){
    int i;
    Node *temp;

    for(i = 0; i < ml->size; i++){
        temp = ml->map[i]->head;
        while(temp != NULL){
            Node *r = temp->next;
            if(temp->entry != NULL)
                me_destroy(temp->entry);
            free(temp);
            temp = r;
        }
        free(ml->map[i]);
    }
    free(ml->map);
    free(ml);

}