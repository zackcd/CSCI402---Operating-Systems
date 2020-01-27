//
//  ListFuncs.c
//  Warmup Assignment 1
//
//  Created by Zachary Downing on 8/24/18.
//  Copyright © 2018 Zachary Downing. All rights reserved.
//

#include "my402list.h"
#include "cs402.h"
#include <stdlib.h>
#include <stdio.h>

int My402ListLength(My402List* list) {
    return list->num_members;
}

int My402ListEmpty(My402List* list) {
    if(My402ListLength(list) == 0)
        return TRUE;
    else
        return FALSE;
}

int My402ListAppend(My402List* list, void* data) {
    if(list == NULL) {
        return FALSE;
    }
    else {
        My402ListElem* node = (My402ListElem*)malloc(sizeof(My402ListElem));
        My402ListElem* last = My402ListLast(list);
        node->obj = data;
        
        if(My402ListEmpty(list)) {
            node->next = &list->anchor;
            node->prev = &list->anchor;
            list->anchor.next = node;
            list->anchor.prev = node;
        }
        else {
            node->next = last->next;
            node->prev = last;
            last->next = node;
            list->anchor.prev = node;
        }
        list->num_members++;
        return TRUE;
        
    }
}

int My402ListPrepend(My402List* list, void* data) {
    if(list == NULL)
        return FALSE;
    else {
        My402ListElem* node = (My402ListElem*)malloc(sizeof(My402ListElem));
        My402ListElem* first = My402ListFirst(list);
        node->obj = data;
        
        if(My402ListEmpty(list)) {
            node->next = &list->anchor;
            node->prev = &list->anchor;
            list->anchor.next = node;
            list->anchor.prev = node;
        }
        else {
            node->next = first;
            node->prev = first->prev;
            first->prev = node;
            list->anchor.next = node;
        }
        list->num_members++;
        return TRUE;
    }
}

void My402ListUnlink(My402List* list, My402ListElem* elem) {
    
    My402ListElem* temp = elem->prev;
    My402ListElem* temp2 = elem->next;
    
    temp->next = elem->next;
    temp2->prev = elem->prev;
    
    list->num_members--;
    
    elem->next = NULL;
    elem->prev = NULL;
    
    free(elem);
}

void My402ListUnlinkAll(My402List* list) {
    if(My402ListEmpty(list))
        return;
    
    else {
        My402ListElem* node = My402ListFirst(list);
        while(node->next != &list->anchor) {
            My402ListElem* temp = node->next;
            My402ListUnlink(list, node);
            node = temp;
        }
    }
    
}

int My402ListInsertAfter(My402List* list, void* data, My402ListElem* elem) {
    if(list == NULL) {
        return FALSE;
    }
    else if(elem == NULL) {
        return My402ListAppend(list, data);
    }
    else if(elem == My402ListLast(list)) {
        return My402ListAppend(list, data);
    }
    else {
        My402ListElem* node = (My402ListElem*)malloc(sizeof(My402ListElem));
        if(node == NULL)
            return FALSE;
        else {
            node->obj = data;
            
            node->next = elem->next;
            node->prev = elem;
            elem->next->prev=node;
            elem->next=node;
            
            
            list->num_members++;
            return TRUE;
        }
    }
}

int My402ListInsertBefore(My402List* list, void* data, My402ListElem* elem) {
    if(list == NULL)
        return FALSE;
    else if(elem == NULL) {
        return My402ListPrepend(list, data);
    }
    else if(elem == My402ListFirst(list)) {
        return My402ListPrepend(list, data);
    }
    else {
        My402ListElem* node = (My402ListElem*)malloc(sizeof(My402ListElem));
        if(node == NULL)
            return FALSE;
        else {
            node->obj = data;
            
            node->next = elem;
            node->prev = elem->prev;
            elem->prev->next = node;
            elem->prev = node;
            
            list->num_members++;
            return TRUE;
        }
    }
}

My402ListElem *My402ListFirst(My402List* list) {
    if(My402ListEmpty(list)) {
        return NULL;
    }
    else {
        return list->anchor.next;
    }
}

My402ListElem *My402ListLast(My402List* list) {
    if(My402ListEmpty(list)) {
        return NULL;
    }
    else {
        return list->anchor.prev;
    }
}

My402ListElem *My402ListNext(My402List* list, My402ListElem* elem) {
    if(elem == My402ListLast(list) || My402ListEmpty(list))
        return NULL;
    else
        return elem->next;
}

My402ListElem *My402ListPrev(My402List* list, My402ListElem* elem) {
    if(elem == My402ListFirst(list) || My402ListEmpty(list))
        return NULL;
    else
        return elem->prev;
}


My402ListElem *My402ListFind(My402List* list, void* data) {
    if(My402ListEmpty(list))
        return NULL;
    else {
        My402ListElem *elem = NULL;
        
        for (elem=My402ListFirst(list); elem != NULL; elem=My402ListNext(list, elem)) {
            if(elem->obj == data) {
                return elem;
            }
        }
        
        return NULL;
    }
}

int My402ListInit(My402List* list) {
    
    if(list == NULL) {
        return FALSE;
    }
    
    else {
        list->num_members = 0;
        
        list->anchor.obj = NULL;
        list->anchor.next = &list->anchor;
        list->anchor.prev = &list->anchor;
        //list->anchor.next = NULL;
        //list->anchor.prev = NULL;
        
        return TRUE;
    }
}
