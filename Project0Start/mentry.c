/*
 * Benjamin Barnes
 * 951289352
 * CIS415 Project 0
 *
 * This is my own work
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "mentry.h"

#define LINESIZE 100

/* me_get returns the next file entry, or NULL if end of file */
MEntry *me_get(FILE *fd){

    char line[LINESIZE] = {0};

    fgets(line, LINESIZE, fd);

    // we have hit the end of the file
    if(line[0] == '\0') return NULL;

    // making space for stuct
    MEntry *entry;

    if((entry = (MEntry *)malloc(sizeof(MEntry))) == NULL) return entry;
    if((entry->surname = malloc(sizeof(char)*100)) == NULL) return entry;
    if((entry->zipcode = malloc(sizeof(char)*20)) == NULL) return entry;
    if((entry->full_address = malloc(sizeof(char)*LINESIZE*3)) == NULL) return entry;

    sprintf(entry->full_address, "%s", line);

    int i,j;

    // getting surname
    for(i = 0; line[i] != ','; i++){
        if(isalpha(line[i])){
            entry->surname[i] = (char)tolower(line[i]);
        }
    }
    // null char on end of surname
    entry->surname[i] = '\0';

    fgets(line, LINESIZE, fd);

    // getting house number
    char* temp;
    temp = (char*)calloc(20, sizeof(char));
    int k;
    for(i = 0, k = 0; ;i++){
        if(line[i] != ' ' && line[i] >= '0' && line[i] <= '9'){
            temp[k] = line[i];
            k++;
        }else if(line[i] == '\n'){
            entry->house_number = atoi(temp);
            break;
        }
    }
    free(temp);

    // writing line to full address
    strcat(entry->full_address, line);

    fgets(line, LINESIZE, fd);

    // getting zip code
    for(i = 0, j = 0; ;i++){
        if(line[i] >= '0' && line[i] <= '9'){
            entry->zipcode[j] = line[i];
            j++;
        }else if(line[i] == '\n'){
            entry->zipcode[j] = '\0';
            break;
        }
    }

    // writing line to full address
    strcat(entry->full_address, line);

    return entry;
}


/* me_hash computes a hash of the MEntry, mod size */
unsigned long me_hash(MEntry *me, unsigned long size){
    int i;
    unsigned long sum = 0;
    sum += (unsigned long)me->house_number;

    for(i = 0; me->zipcode[i] != '\0'; i++){
        sum += (unsigned long)(me->zipcode[i]) * (i+1) * 13;

    }
    for(i = 0; me->surname[i] != '\0'; i++){
        sum += (unsigned long)(me->surname[i]) * (i+1) * 13;
    }

    return (sum % size);
}

/*me_print prints the full address on fd */
void me_print(MEntry *me, FILE *fd){
    fprintf(fd, "%s", me->full_address);
}

/* me_compare compares two mail entries, returning <0, 0, >0 if
 * me1<me2, me1==me2, me1>me2
 */
int me_compare(MEntry *me1, MEntry *me2){
    if((strcmp(me1->surname, me2->surname) == 0) && (strcmp(me1->zipcode, me2->zipcode) == 0) && (me1->house_number == me2->house_number)){
        return 0;
    }else if(me1->house_number < me2->house_number){
        return -1;
    }else{
        return 1;
    }
}

/* me_destroy destroys the mail entry */
void me_destroy(MEntry *me){
    if(me != NULL) {
        free(me->surname);
        free(me->full_address);
        free(me->zipcode);
    }
    free(me);
}