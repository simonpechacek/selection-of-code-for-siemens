#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "linked_list.h"

#define MEMMORY_ERROR -1
#define L_LIST_EMPTY -1
#define ENTRY_NOT_FOUND -1

typedef struct linked_list
{
    int size;
    struct entry *first;

} L_list;

typedef struct entry
{
    int value;
    struct entry *next;
} Entry;

//---------FUNCTION DECLARATION------------//
Entry *new_entry(int value);
void print_q();
//------------------END-------------------//

//------CREATE AND INITIALIZE LINKED LIST------//
L_list l_list = {.size = 0, .first = NULL};
/*
int main()
{
    push(10);
    push(8);
    push(6);
    push(10);
    push(4);
    push(2);
    push(10);
    print_q();
    printf("\n");
    insert(9);
    insert(7);
    insert(5);
    insert(3);
    print_q();
    printf("\n");
    erase(10);
    print_q();
    printf("\n");
    return 0;
}
*/
_Bool push(int entry)
{
    // adds value 'entry' on the end of the q (tail)
    bool succes = false;
    if (entry >= 0)
    {
        Entry *n_entry = new_entry(entry);
        if (l_list.size == 0)
            l_list.first = n_entry;
        else
        {
            Entry *iter = l_list.first;
            while (iter->next != NULL)
                iter = iter->next;
            iter->next = n_entry;
        }
        succes = true;
        l_list.size++;
    }
    return succes;
}

int pop(void)
{
    // removes one value from the tail
    int ret = L_LIST_EMPTY;
    if (l_list.size > 0)
    {
        Entry *head = l_list.first;
        l_list.first = head->next;
        l_list.size--;
        ret = head->value;
        free(head);
    }
    return ret;
}

_Bool insert(int entry)
{
    // inserts 'entry' value before first smaller(or equal) value
    bool succes = false;
    if (entry >= 0)
    {
        Entry *n_entry = new_entry(entry);
        if (l_list.size == 0)
        {
            l_list.first = n_entry;
        }
        else if (l_list.first->value < n_entry->value)
        {
            n_entry->next = l_list.first;
            l_list.first = n_entry;
        }
        else
        {
            Entry *iter = l_list.first;
            while (iter->next != NULL && iter->next->value > n_entry->value)
                iter = iter->next;
            n_entry->next = iter->next;
            iter->next = n_entry;
        }
        l_list.size++;
        succes = true;
    }
    return succes;
}

_Bool erase(int entry)
{
    //erases all occurances of 'entry' value
    bool succes = false;
    if (entry >= 0 && l_list.size > 0)
    {
        Entry *iter = l_list.first;

        while (true)
        {
            if (iter == NULL)
                break;

            if (l_list.first->value == entry)
            {
                Entry *head = l_list.first;
                l_list.first = l_list.first->next;
                iter = l_list.first;
                free(head);
                succes = true;
                l_list.size--;
            }
            else
            {
                if (iter->next != NULL)
                {
                    if (iter->next->value == entry)
                    {
                        Entry *to_be_deleted = iter->next;
                        iter->next = to_be_deleted->next;
                        free(to_be_deleted);
                        succes = true;
                        l_list.size--;
                    }
                    else
                        iter = iter->next;
                }
                else
                    break;
            }
        }
    }
    return succes;
}

int getEntry(int idx)
{
    // returns entry on idx-th position (zero indexed)
    int ret = ENTRY_NOT_FOUND;
    if (idx >= 0 && idx < l_list.size)
    {
        int counter = 0;
        Entry *iter = l_list.first;
        while (iter != NULL && counter != idx)
        {
            counter++;
            iter = iter->next;
        }
        ret = iter->value;
    }
    return ret;
}

int size(void)
{
    //returns currrnt size of q
    return l_list.size;
}
void clear()
{
    // clears out and frees whole q
    Entry *iter = l_list.first;
    while (iter != NULL)
    {
        Entry *to_be_deleted = iter;
        iter = iter->next;
        free(to_be_deleted);
        l_list.size--;
    }
    l_list.first = NULL;
}

Entry *new_entry(int value)
{
    // creates new instance of entry struct and initialize it
    Entry *n_entry = (struct entry *)malloc(sizeof(struct entry));
    if (n_entry == NULL)
    {
        fprintf(stderr, "Malloc error!\n");
        exit(MEMMORY_ERROR);
    }
    n_entry->value = value;
    n_entry->next = NULL;

    return n_entry;
}

void print_q()
{
    // prints out whole q 
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