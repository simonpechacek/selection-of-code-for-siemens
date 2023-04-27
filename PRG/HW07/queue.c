#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
// TODO - your code
#define MEMMORY_ERROR -1
#define POP_FAIL NULL
#define ENTRY_NOT_FOUND NULL
#define WRONG_Q_POINTER -1


typedef struct data_entry
{
    void *data;
    struct data_entry *next;
} Entry;

typedef struct
{
    int size;
    int (*cmp)(const void *, const void *);
    void (*clr)(void *);
    Entry *first;
} L_list;



Entry *new_entry(void *data);

void *create()
{
    // creates new linked_list type q -> if malloc fails returns NULL
    L_list *new_q = (L_list *)malloc(sizeof(L_list));
    if (new_q != NULL)
    {
        new_q->cmp = NULL;
        new_q->clr = NULL;
        new_q->size = 0;
        new_q->first = NULL;
    }
    return (void *)new_q;
}

void clear(void *queue)
{
    if (queue != NULL)
    {
        L_list *l_list = (L_list *)queue;
        Entry *iter = l_list->first;
        while (iter != NULL)
        {
            Entry *to_be_deleted = iter;
            iter = iter->next;
            if (l_list->clr != NULL)
                l_list->clr(to_be_deleted->data);
            else
                free(to_be_deleted->data);
            free(to_be_deleted);
            l_list->size--;
        }
        l_list->first = NULL;
    }
}

_Bool push(void *queue, void *entry)
{
    L_list *l_list = (L_list *)queue;
    bool succes = false;
    if (queue != NULL && entry != NULL)
    {
        Entry *n_entry = new_entry(entry);
        if (l_list->size == 0)
            l_list->first = n_entry;
        else
        {
            Entry *iter = l_list->first;
            while (iter->next != NULL)
                iter = iter->next;
            iter->next = n_entry;
        }
        succes = true;
        l_list->size++;
    }
    return succes;
}

void *pop(void *queue)
{
    void *ret = POP_FAIL;
    L_list *l_list = (L_list *)queue;
    if (queue != NULL && l_list->size > 0)
    {
        Entry *head = l_list->first;
        l_list->first = head->next;
        l_list->size--;
        ret = head->data;
        free(head);
    }
    return ret;
}

_Bool insert(void *queue, void *entry)
{
    bool succes = false;
    L_list *l_list = (L_list *)queue;
    if (queue != NULL && l_list->cmp != NULL)
    {
        Entry *n_entry = new_entry(entry);
        if (l_list->size == 0)
        {
            l_list->first = n_entry;
        }
        else if (l_list->cmp(l_list->first->data, n_entry->data) <= 0)
        {
            n_entry->next = l_list->first;
            l_list->first = n_entry;
        }
        else
        {
            Entry *iter = l_list->first;
            while (iter->next != NULL && l_list->cmp(iter->next->data, n_entry->data) > 0)
                iter = iter->next;
            n_entry->next = iter->next;
            iter->next = n_entry;
        }
        l_list->size++;
        succes = true;
    }
    return succes;
}

_Bool erase(void *queue, void *entry)
{
    bool succes = false;
    L_list *l_list = (L_list *)queue;
    if (l_list->size > 0 && l_list->cmp != NULL && entry != NULL && queue != NULL)
    {
        Entry *iter = l_list->first;

        while (true)
        {
            if (iter == NULL)
                break;

            if (l_list->cmp(l_list->first->data, entry) == 0)
            {
                Entry *head = l_list->first;
                l_list->first = l_list->first->next;
                iter = l_list->first;
                if (l_list->clr != NULL)
                    l_list->clr(head->data);
                else
                    free(head->data);
                free(head);
                succes = true;
                l_list->size--;
            }
            else
            {
                if (iter->next != NULL)
                {
                    if (l_list->cmp(iter->next->data, entry) == 0)
                    {
                        Entry *to_be_deleted = iter->next;
                        iter->next = to_be_deleted->next;
                        if (l_list->clr != NULL)
                            l_list->clr(to_be_deleted->data);
                        else
                            free(to_be_deleted->data);
                        free(to_be_deleted);
                        succes = true;
                        l_list->size--;
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

void *getEntry(const void *queue, int idx)
{
    void *ret = ENTRY_NOT_FOUND;
    L_list *l_list = (L_list *)queue;
    if (idx >= 0 && idx < l_list->size && queue != NULL)
    {
        int counter = 0;
        Entry *iter = l_list->first;
        while (iter != NULL && counter != idx)
        {
            counter++;
            iter = iter->next;
        }
        ret = iter->data;
    }
    return ret;
}

int size(const void *queue)
{
    int q_size = WRONG_Q_POINTER;
    if (queue != NULL)
    {
        L_list *l_list = (L_list *)queue;
        q_size = l_list->size;
    }
    return q_size;
}

void setCompare(void *queue, int (*compare)(const void *, const void *))
{
    if (queue != NULL)
    {
        L_list *l_list = (L_list *)queue;
        l_list->cmp = compare;
    }
}

void setClear(void *queue, void (*clear)(void *))
{
    if (queue != NULL)
    {
        L_list *l_list = (L_list *)queue;
        l_list->clr = clear;
    }
}

Entry *new_entry(void *data)
{
    // creates new instance of entry struct and initialize it
    Entry *n_entry = (struct data_entry *)malloc(sizeof(struct data_entry));
    if (n_entry == NULL)
    {
        fprintf(stderr, "Malloc error!\n");
        exit(MEMMORY_ERROR);
    }
    n_entry->data = data;
    n_entry->next = NULL;

    return n_entry;
}