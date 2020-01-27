#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "cs402.h"
#include "my402list.h"
#include "listFuncs.h"


FILE* readFile() {
    static const char filename[] = "test2.txt";
    FILE *fp = fopen (filename, "r");
    return fp;
}

int absVal(int n) {
    if(n >= 0) {
        return n;
    }
    else {
        n -= 2*n;
        return n;
    }
}

void procAndCheckSign(transactionItem* item, char* sign) {
    if(*sign != '-' && *sign != '+') {
        printError("sign char not valid");
    }
    else {
        readSign(item, *sign);
    }
}

void procAndCheckTimeStamp(transactionItem* item, char* time) {
    if(strnlen(time, MAX_TIMESTAMP_LENGTH) != 10) {
        printError("timestamp incorrect length");
    }
    else {
        readTimeStamp(item, time);
    }
}

void procAndCheckTransAmount(transactionItem* item, char* amount) {
    readTransAmount(item, amount);
}

void procAndCheckDescription(transactionItem* item, char* desc) {
    if(desc == NULL) {
        printError("no description provided");
    }
    else {
        readDescription(item, desc);
    }
}

void processLine(transactionItem* item, char* line) {
    int i = 0;

    char *start_ptr = line;
    while(start_ptr != NULL) {
        char *tab_ptr = strchr(start_ptr, '\t');
        if (tab_ptr != NULL) {
            *tab_ptr++ = '\0';
        }
        line = start_ptr;
        start_ptr = tab_ptr;
        /*
        fputs("line: ", stdout);
        fputs(line, stdout);
        fputs("\n", stdout);
        */
        
        if(i == 0) {
            procAndCheckSign(item, line);
        }
        else if(i == 1) {
            procAndCheckTimeStamp(item, line);
        }
        else if(i == 2) {
            procAndCheckTransAmount(item, line);
        }
        else if(i == 3) {
            procAndCheckDescription(item, line);
        }
        else {
            printError("exceeded correct number of items in a line");
        }
        i++;
    }
    
}

static
void BubbleForward(My402List *pList, My402ListElem **pp_elem1, My402ListElem **pp_elem2)
/* (*pp_elem1) must be closer to First() than (*pp_elem2) */
{
    My402ListElem *elem1=(*pp_elem1), *elem2=(*pp_elem2);
    transactionItem* obj1=elem1->obj;
    transactionItem* obj2=elem2->obj;
    My402ListElem *elem1prev=My402ListPrev(pList, elem1);
    /*  My402ListElem *elem1next=My402ListNext(pList, elem1); */
    /*  My402ListElem *elem2prev=My402ListPrev(pList, elem2); */
    My402ListElem *elem2next=My402ListNext(pList, elem2);
    
    My402ListUnlink(pList, elem1);
    My402ListUnlink(pList, elem2);
    if (elem1prev == NULL) {
        (void)My402ListPrepend(pList, obj2);
        *pp_elem1 = My402ListFirst(pList);
    } else {
        (void)My402ListInsertAfter(pList, obj2, elem1prev);
        *pp_elem1 = My402ListNext(pList, elem1prev);
    }
    if (elem2next == NULL) {
        (void)My402ListAppend(pList, obj1);
        *pp_elem2 = My402ListLast(pList);
    } else {
        (void)My402ListInsertBefore(pList, obj1, elem2next);
        *pp_elem2 = My402ListPrev(pList, elem2next);
    }
}

static
void BubbleSortForwardList(My402List *pList, int num_items)
{
    My402ListElem *elem=NULL;
    int i=0;
    transactionItem* item1;
    transactionItem* item2;
    
    
    if (My402ListLength(pList) != num_items) {
        printError("Error: list length unable to be read");
    }
    for (i=0; i < num_items; i++)
    {
        int j=0, something_swapped=FALSE;
        My402ListElem* next_elem=NULL;
        //char item1Cpy[MAX_TIMESTAMP_LENGTH];
        //char item2Cpy[MAX_TIMESTAMP_LENGTH];
        
        for (elem=My402ListFirst(pList), j=0; j < num_items-i-1; elem=next_elem, j++)
        {
            item1 = (transactionItem*)(elem->obj);
            /*
            strncpy(item1Cpy, item1->timeStamp, sizeof(item1Cpy));
            int cur_val = atoi(item1Cpy);
             */
            //int cur_val = atoi(item1->timeStamp);
            int cur_val = item1->timeStamp;

            next_elem=My402ListNext(pList, elem);

            item2 = (transactionItem*)(next_elem->obj);
            /*
            strncpy(item2Cpy, item2->timeStamp, sizeof(item2Cpy));
            int next_val = atoi(item2Cpy);
             */
            //int next_val = atoi(item2->timeStamp);
            int next_val = item2->timeStamp;

            if (cur_val > next_val) {
                BubbleForward(pList, &elem, &next_elem);
                something_swapped = TRUE;
            }

        }
        if (!something_swapped) break;
    }
}


void printLine(transactionItem* item, int* balance) {
    printDate(item);
    printDesc(item);
    int amount = stringToCents(item->transAmount);
    int transSign = getSign(item);
    printAmount(amount, transSign);
    *balance += (amount * transSign);
    int balSign;
    if(*balance > 0) {
        balSign = 1;
    }
    else {
        balSign = -1;
    }
    printAmount(absVal(*balance), balSign);
    fputs(" |", stdout);
    fputs("\n", stdout);
    fputs("+-----------------+--------------------------+----------------+----------------+\n", stdout);
}

void insert(My402List* list, transactionItem* item) {
    if(My402ListEmpty(list)) {
        My402ListAppend(list, item);
    }
    else {
        
        //char* timeCpy = (char*)malloc(sizeof(item->timeStamp));
        //strncpy(timeCpy, item->timeStamp, MAX_TIMESTAMP_LENGTH);
        transactionItem* lastItem = (transactionItem*)My402ListLast(list)->obj;
        //char* timeLastCpy = (char*)malloc(sizeof(item->timeStamp));
        //strncpy(timeLastCpy, lastItem->timeStamp, MAX_TIMESTAMP_LENGTH);
        //if(strncmp(timeCpy, timeLastCpy, MAX_TIMESTAMP_LENGTH) == 0) {
        if(item->timeStamp == lastItem->timeStamp) {
            printError("error: identical timestamps found");
        }
        else {
            My402ListAppend(list, item);
        }
        //free(timeCpy);
        //free(timeLastCpy);
    }
}

void printStatement(My402List * list) {
    fputs("+-----------------+--------------------------+----------------+----------------+\n", stdout);
    fputs("|       Date      | Description              |         Amount |        Balance |\n", stdout);
    fputs("+-----------------+--------------------------+----------------+----------------+\n", stdout);
    int balance = 0;
    My402ListElem* node = My402ListFirst(list);
    while(node != &list->anchor) {
        printLine(node->obj, &balance);
        node = node->next;
    }
    fputs("\n", stdout);
}

void processList(FILE* fp) {
    My402List* list = (My402List*)(malloc(sizeof(My402List)));
    My402ListInit(list);
    int numItems = 0;
    
    char buf[BUFFER_LENGTH];
    while(fgets(buf, sizeof(buf), fp) != NULL) {
        transactionItem* item = (transactionItem*)malloc(sizeof(transactionItem));
        processLine(item, buf);
        insert(list, item);
        numItems++;
    }
    BubbleSortForwardList(list, numItems);
    printStatement(list);
    My402ListUnlinkAll(list);
    free(list);
}

//int main(int argc, char *argv[])
int main()
{
    FILE *fp = readFile();
    if (fp != NULL)
    {
        processList(fp);
    }
    else
    {
        printError("error: unable to open file");
    }
    return 0;
}
