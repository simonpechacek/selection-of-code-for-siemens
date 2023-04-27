#include "queue.h"

#define MEMMORY_ERROR -1

// ADITIONAL FUNCTIONS
bool is_full(queue_t *queue);
bool is_empty(queue_t *queue);
int get_queue_size(queue_t *queue);
// OPTIONAL HW
void realloc_queue(queue_t *queue, size_t new_size);
void shift_queue(queue_t *queue);

int main(void)
{
    queue_t *q = create_queue(4);
    int a, b, c, d, e, f, g;
    a = 1;
    b = 2;
    c = 3;
    d = 4;
    e = 5;
    f = 6;
    g = 7;
    push_to_queue(q, (void *)&a);
    push_to_queue(q, (void *)&b);
    push_to_queue(q, (void *)&c);
    push_to_queue(q, (void *)&d);

    for (int i = 0; i < 4; ++i)
    {
        printf("%d ", *(int *)q->data[i]);
    }
    printf("\n");
    pop_from_queue(q);
    //push_to_queue(q, (void *)&e);
    shift_queue(q);
    push_to_queue(q, (void *)&f);
    for (int i = 0; i < 4; ++i)
    {
        printf("%d ", *(int *)q->data[i]);
    }
    printf("\n");

    return 0;
}

queue_t *create_queue(int capacity)
{
    // creates queue (dynamically allocated)
    queue_t *que = (queue_t *)malloc(sizeof(queue_t));
    void **data = (void **)malloc(capacity * sizeof(void *));
    if (que == NULL || data == NULL)
    {
        fprintf(stderr, "Malloc Error!\n");
        exit(MEMMORY_ERROR);
    }

    que->first = -1;
    que->last = -1;
    que->size = capacity;
    que->data = data;

    return que;
}

void delete_queue(queue_t *queue)
{
    // deletes queue and frees all allocated memmory
    queue->first = -1;
    queue->last = -1;
    queue->size = 0;
    free(queue->data);
    free(queue);
}

bool push_to_queue(queue_t *queue, void *data)
{
    // pushes element to queue -> if queue is full make it bigger
    bool succes = true;
    if (queue->first == -1)
    {
        // Empty que just place it on the first index
        queue->first++;
        queue->last++;
        queue->data[queue->last] = data;
    }
    else if (!is_full(queue))
    {
        queue->last = (queue->last + 1) % queue->size;
        queue->data[queue->last] = data;
    }
    else
        succes = false;

    return succes;
}

void *pop_from_queue(queue_t *queue)
{
    // pops first element from queue
    void *value = NULL;
    if (!is_empty(queue))
    {
        value = queue->data[queue->first];
        if (queue->first == queue->last)
        {
            // now the que is empty
            queue->first = -1;
            queue->last = -1;
        }
        else
            queue->first = (queue->first + 1) % queue->size;
    }
    return value;
}

void *get_from_queue(queue_t *queue, int idx)
{
    // gets element on index: idx from queue
    void *value = NULL;
    int queue_size = get_queue_size(queue);
    if (idx >= 0 && idx < queue_size)
    {
        int value_index = (queue->first + idx) % queue->size;
        value = queue->data[value_index];
    }
    return value;
}

int get_queue_size(queue_t *queue)
{
    // returns the number of elements in queue
    int size;
    int dt = queue->last - queue->first;
    if (queue->first == -1)
        size = 0;
    else if (dt >= 0)
        size = dt + 1;
    else
        size = queue->size - dt + 1;
    return size;
}

bool is_empty(queue_t *queue)
{
    // returns wheter the que is empty or not
    bool empty = false;
    if (queue->first == -1)
        empty = true;
    return empty;
}

bool is_full(queue_t *queue)
{
    // returns wheter the que is full or not
    bool full = false;
    if ((queue->first == 0 && queue->last == queue->size - 1) || queue->first == queue->last + 1)
    {
        full = true;
    }
    return full;
}

// Optional HW

void realloc_queue(queue_t *queue, size_t new_size)
{
    // reallocs the queue to: new_size
    void *tmp = realloc(queue->data, new_size);
    if (tmp != NULL)
    {
        queue->data = (void **)tmp;
        tmp = NULL;
    }
    else
    {
        fprintf(stderr, "Realloc Error!\n");
        exit(MEMMORY_ERROR);
    }
}

void shift_queue(queue_t *queue)
{
    // loads all data to tmp array
    // moves it to the start of queue for realloc
    int q_size = get_queue_size(queue);
    if (q_size > 0)
    {
        void **tmp = (void **)malloc(q_size * sizeof(void *));
        for (int i = 0; i < q_size; ++i)
        {
            tmp[i] = queue->data[((queue->first + i) % queue->size)];
        }
        for (int i = 0; i < q_size; ++i)
        {
            queue->data[i] = tmp[i];
        }
        free(tmp);
        queue->first = 0;
        queue->last = q_size - 1;
    }
}
