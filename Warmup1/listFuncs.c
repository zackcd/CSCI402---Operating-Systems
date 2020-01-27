//
//  listFuncs.c
//  Warmup Assignment 1 Part B
//
//  Created by Zachary Downing on 8/29/18.
//  Copyright © 2018 Zachary Downing. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "cs402.h"
#include "my402list.h"
#include "listFuncs.h"


void printError(char* errorMessage) {
    perror(errorMessage);
    exit(1);
}

void addCommas(char* head, char* str) {
    char *ptr, *optr;
    long length, commas;
    
    for (ptr = head; *ptr; ptr++);
    
    length = ptr - head;
    commas = ( length - 1 ) / 3;
    optr = str + length + commas;
    
    *optr-- = *ptr--;
    for (int i = 1; ptr >= head; i++)
    {
        *optr-- = *ptr--;
        if ((i % 3) == 0)
            *optr-- = ',';
    }
}

int stringToCents(char* str) {
    int cents = 0;
    char copy[MAX_AMOUNT_LENGTH];
    copy[0] = '\0';
    //strncpy(copy, item->transAmount, sizeof(copy));
    strncpy(copy, str, sizeof(copy));
    char *head = copy;
    char *dec_ptr = strchr(head, '.');
    *dec_ptr = '\0';
    int dollars = atoi(head) * 100;
    cents += atoi(++dec_ptr);
    return cents + dollars;
}

void centsToString(char buf[10], int value) {
    int dollars = value / 100;
    int cents = value;
    while(cents > 100) {
        cents = cents % 100;
    }
/*
    char* dollarsStr;
    sprintf(dollarsStr, "%d", dollars);
    char* centsStr;
    sprintf(centsStr, "%d", cents);
 */

    if(cents % 10 == 0) {
        sprintf(buf, "%d.%d0", dollars, cents);
    }
    else if(cents < 10) {
        sprintf(buf, "%d.0%d", dollars, cents);
    }
    else {
        sprintf(buf, "%d.%d", dollars, cents);
    }
    
}

int getSign(transactionItem* item) {
    char sign = item->sign;
    int multiplier = 0;
    if(sign == '-') {
        multiplier = -1;
    }
    else if(sign == '+') {
        multiplier = 1;
    }
    else {
        printError("invalid sign");
    }
    return multiplier;
}

void readSign(transactionItem* item, char s) {
    strncpy(&item->sign, &s, sizeof(item->sign));
}
void readTimeStamp(transactionItem* item, char time[MAX_TIMESTAMP_LENGTH]) {
    //strncpy(item->timeStamp, time, MAX_TIMESTAMP_LENGTH);
    item->timeStamp = atoi(time);
}
void readTransAmount(transactionItem* item, char amount[MAX_AMOUNT_LENGTH]) {
    strncpy(item->transAmount, amount, sizeof(item->transAmount));
}
void readDescription(transactionItem* item, char desc[BUFFER_LENGTH]) {
    strncpy(item->description, desc, sizeof(item->description));
}

//Functions to print correctly formatted data
void printDate(transactionItem* item) {
    char timeCpy[MAX_TIMESTAMP_LENGTH];
    timeCpy[0] = '\0';
    //time_t time = atoi(item->timeStamp);
    //strncpy(timeCpy, item->timeStamp, sizeof(timeCpy));
    //time_t time = atoi(timeCpy);
    time_t time = item->timeStamp;
    
    char timeBuf[26];
    char formattedDate[16];
    strncpy(timeBuf, ctime(&time), sizeof(timeBuf));
    for(int i = 0; i < 11; i++) {
        formattedDate[i] = timeBuf[i];
    }
    for(int i = 11; i < 15; i++) {
        formattedDate[i] = timeBuf[i+9];
    }
    //formattedDate[15] = '\0';
    fputs("| ", stdout);
    fputs(formattedDate, stdout);
    
}

void printDesc(transactionItem* item) {
    char descBuf[BUFFER_LENGTH];
    descBuf[0] = '\0';
    char formattedDesc[24];
    formattedDesc[0] = '\0';
    for(int i = 0; i < 24; i++) {
        formattedDesc[i] = ' ';
    }
    strncpy(descBuf, item->description, sizeof(descBuf));
    int i = 0;
    while(descBuf[i] == ' ') {
        i++;
    }
    for(int j = 0; j < 23; j++) {
        if(descBuf[i + j] == '\n') {
            break;
        }
        else {
            formattedDesc[j] = descBuf[i + j];
        }
    }

    fputs(" | ", stdout);
    fputs(formattedDesc, stdout);
}

void printAmount(int amount, int sign) {
    //13 chars
    char amountBuf[MAX_AMOUNT_LENGTH];
    amountBuf[0] = '\0';
    char formattedAmount[15];
    formattedAmount[0] = '\0';
    int negative;
    if(sign == -1) {
        negative = TRUE;
    }
    else {
        negative = FALSE;
    }

    if(amount >= 1000000000) {
        strncpy(formattedAmount, "?,???,???.??", MAX_AMOUNT_LENGTH + 2);
    }
    else {
        centsToString(amountBuf, amount);
        char copy[MAX_AMOUNT_LENGTH];
        copy[0] = '\0';
        //strncpy(copy, item->transAmount, sizeof(copy));
        strncpy(copy, amountBuf, sizeof(copy));
        char *head = copy;
        char *dec_ptr = strchr(head, '.');
        *dec_ptr = '\0';
    
        char amountCommas[MAX_AMOUNT_LENGTH];
        addCommas(head, amountCommas);
        
        *dec_ptr = '.';
        
        /*
        char *end_ptr = dec_ptr;

        *end_ptr++;
        *end_ptr++;
        *end_ptr++;
        *end_ptr = '\0';
        */
        strncpy(formattedAmount, amountCommas, sizeof(formattedAmount));
        strncat(formattedAmount, dec_ptr, sizeof(formattedAmount));
    }
    
    fputs(" | ", stdout);
    if(negative) {
        fputs("(", stdout);
    }
    else {
        fputs(" ", stdout);
    }
    int numSpaces = 12;
    printf("%*s",numSpaces,formattedAmount);
    if(negative) {
        fputs(")", stdout);
    }
    else {
        fputs(" ", stdout);
    }
}
