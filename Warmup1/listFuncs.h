//
//  listFuncs.h
//  Warmup Assignment 1 Part B
//
//  Created by Zachary Downing on 8/29/18.
//  Copyright © 2018 Zachary Downing. All rights reserved.
//

#ifndef listFuncs_h
#define listFuncs_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "cs402.h"
#include "my402list.h"

#define OVER_10_MILL "?,???,???.??"

#ifndef LINE_LENGTH
#define LINE_LENGTH 80
#endif

#ifndef MAX_SIGN_LENGTH
#define MAX_SIGN_LENGTH 1
#endif

#ifndef MAX_TIMESTAMP_LENGTH
#define MAX_TIMESTAMP_LENGTH 10
#endif

#ifndef MAX_AMOUNT_LENGTH
#define MAX_AMOUNT_LENGTH 10
#endif

#ifndef BUFFER_LENGTH
#define BUFFER_LENGTH 1024
#endif

typedef struct tagTransactionItem {
    char sign;
    //char timeStamp[MAX_TIMESTAMP_LENGTH];
    int timeStamp;
    char transAmount[MAX_AMOUNT_LENGTH];
    char description[BUFFER_LENGTH];
} transactionItem;

void printError(char* errorMessage);
int stringToCents(char* str);
int getSign(transactionItem* item);
void centsToString(char* buf, int value);
extern void readSign(transactionItem* item, char s);
extern void readTimeStamp(transactionItem* item, char time[MAX_TIMESTAMP_LENGTH]);
extern void readTransAmount(transactionItem* item, char amount[MAX_AMOUNT_LENGTH]);
extern void readDescription(transactionItem* item, char desc[BUFFER_LENGTH]);
extern void printDate(transactionItem* item);
extern void printDesc(transactionItem* item);
//extern void printAmount(transactionItem* item);
//extern void printAmount(char* string, char sign);
void printAmount(int amount, int sign);
extern void printList(My402List* list);

#endif /* listFuncs_h */
