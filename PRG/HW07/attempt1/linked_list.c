#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "linked_list.h"

#define MEMMORY_ERROR -1
#define L_LIST_EMPTY -1
#define NOT_IN_Q -1

// Linked list will keep track of its size
// also about first and last entry
typedef struct linked_list
{
    int size;
    struct entry *first;
    struct entry *last;

} L_list;

typedef struct entry
{
    int value;
    struct entry *next;
    struct entry *prev;
} Entry;

//---------FUNCTION DECLARATION------------//
Entry *new_entry(int value);
void print_q();
//------------------END-------------------//

//------CREATE AND INITIALIZE LINKED LIST------//
L_list l_list = {.size = 0, .first = NULL, .last = NULL};

//--------Linked list functionality---------//
_Bool push(int entry)
{
    bool succes = false;
    if (entry >= 0)
    {
        Entry *n_entry = new_entry(entry);
        if (l_list.size == 0)
        {
            l_list.first = n_entry;
            l_list.last = n_entry;
        }
        else
        {
            n_entry->prev = l_list.last;
            l_list.last->next = n_entry;
            //printf("%p\n", (void *)l_list.last->next->prev);
            l_list.last = n_entry;
        }
        l_list.size++;
        succes = true;
        printf("last %p, prev %p\n", (void*)l_list.last, (void*)l_list.last->prev->next);
        printf("Q {%d}= ", l_list.size);
        print_q();
        printf("first %d, last %d", l_list.first->value, l_list.last->value);
        printf("\n");
    }
    return succes;
}

int pop(void)
{
    int ret = L_LIST_EMPTY;
    if (l_list.size > 0)
    {
        Entry *first = l_list.first;
        printf("first ->self %p ->next->prev %p\n", (void *)l_list.first, (void *)l_list.first->next->prev);
        l_list.first = l_list.first->next;
        printf("new first: %d...after that %d\n", l_list.first->value, l_list.first->next->value);
        if (l_list.first != NULL)
            l_list.first->prev = NULL;
        // printf("new first: %d...after that %d\n", l_list.first->value, l_list.first->next->value);

        l_list.size--;
        ret = first->value;

        printf("Q {%d}= ", l_list.size);
        print_q();
        printf("first %d, last %d", l_list.first->value, l_list.last->value);
        printf("\n");

        free(first);
    }
    return ret;
}

_Bool insert(int entry)
{
    bool succes = false;

    if (entry >= 0)
    {
        Entry *n_entry = new_entry(entry);
        if (l_list.size == 0)
        {
            l_list.first = n_entry;
            l_list.last = n_entry;
        }
        else
        {
            Entry *next = l_list.first;
            while (next != NULL && next->value > entry)
                next = next->next;

            if (next == NULL)
            {
                n_entry->prev = l_list.last;
                l_list.last->next = n_entry;
                l_list.last = n_entry;
            }
            else
            {
                if (next->prev == NULL)
                {
                    next->prev = n_entry;
                    n_entry->next = next;
                    l_list.first = n_entry;
                }
                else
                {
                    next->prev->next = n_entry;
                    n_entry->prev = next->prev;
                    n_entry->next = next;
                    next->prev = n_entry;
                }
            }
        }
        l_list.size++;
    }
    return succes;
}

_Bool erase(int entry)
{
    bool succes = false;
    if (l_list.size > 0)
    {
        Entry *next = l_list.first;
        while (next != NULL)
        {
            if (next->value == entry)
            {
                if (next == l_list.first)
                    l_list.first = next->next;
                else if (next == l_list.last)
                    l_list.last = next->prev;
                if (next->next != NULL)
                    next->next->prev = next->prev;
                if (next->prev != NULL)
                    next->prev->next = next->next;
                Entry *to_be_deleted = next;
                next = next->next;
                free(to_be_deleted);
                l_list.size--;
            }
            else
                next = next->next;
        }
    }
    return succes;
}

int getEntry(int idx)
{
    int ret = NOT_IN_Q;
    if (idx >= 0 && idx < l_list.size)
    {
        int idx_cnt = 0;
        Entry *next = l_list.first;
        while (next != NULL && idx_cnt != idx)
        {
            next = next->next;
            idx_cnt++;
        }
        if (idx_cnt == idx)
            ret = next->value;
    }
    return ret;
}

int size(void)
{
    return l_list.size;
}

void clear()
{
    Entry *next = l_list.first;
    while (next != NULL)
    {
        Entry *to_be_deleted = next;
        next = next->next;
        free(to_be_deleted);
    }
    l_list.first = NULL;
    l_list.last = NULL;
    l_list.size = 0;
}

// ADITIONAL FUNCTIONS//

Entry *new_entry(int value)
{
    Entry *n_entry = (struct entry *)malloc(sizeof(new_entry));
    if (n_entry == NULL)
    {
        fprintf(stderr, "Malloc error!\n");
        exit(MEMMORY_ERROR);
    }
    n_entry->value = value;
    n_entry->next = NULL;
    n_entry->prev = NULL;

    return n_entry;
}

void print_q()
{
    if (l_list.size > 0)
    {
        Entry *next = l_list.first;
        while (next != NULL)
        {
            printf("%d ", next->value);
            next = next->next;
        }
    }
}