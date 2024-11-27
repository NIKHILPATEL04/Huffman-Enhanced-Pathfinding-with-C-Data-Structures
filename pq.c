#include "pq.h"

#include "node.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ListElement ListElement;

struct ListElement {
    Node *tree;
    ListElement *next;
};
struct PriorityQueue {
    ListElement *list;
};

PriorityQueue *pq_create(void) {
    PriorityQueue *Queue = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (Queue == NULL) {
        return NULL;
    }
    Queue->list = NULL;
    return Queue;
}

bool pq_is_empty(PriorityQueue *q) {
    if (q->list == NULL) {
        return true;
    } else {
        return false;
    }
}

bool pa_less_than(ListElement *e1, ListElement *e2) {
    if (e1 == NULL || e2 == NULL) {
        return false;
    }
    return e1->tree->weight < e2->tree->weight;
}

void pq_free(PriorityQueue **q) {
    free(*q);
    *q = NULL;
}
void enqueue(PriorityQueue *q, Node *tree) {
    ListElement *xx = (ListElement *) malloc(sizeof(ListElement));
    if (xx == NULL) {
        return;
    }
    xx->next = NULL;
    xx->tree = tree;

    if (q->list == NULL) {
        q->list = xx;
    } else if (pa_less_than(xx, q->list)) {
        xx->next = q->list;
        q->list = xx;
    } else {
        ListElement *x = q->list;
        while (x->next != NULL && !pa_less_than(xx, x->next)) {
            x = x->next;
        }
        xx->next = x->next;
        x->next = xx;
    }
}

Node *dequeue(PriorityQueue *q) {
    if (q->list == NULL || q == NULL) {
        printf("Error:\n");
        exit(EXIT_FAILURE);
    }

    Node *oldnode = q->list->tree;

    ListElement *newnode = q->list;
    q->list = q->list->next;
    free(newnode);

    return oldnode;
}
bool pq_size_is_1(PriorityQueue *q) {
    if (q != NULL && q->list != NULL && q->list->next == NULL) {
        return true;
    }
    return false;
}

void pq_print(PriorityQueue *q) {
    assert(q != NULL);
    ListElement *e = q->list;
    int position = 1;
    while (e != NULL) {
        if (position++ == 1) {
            printf("============================================\n");
        } else {
            printf("--------------------------------------------\n");
        }
        node_print_tree(e->tree);
        e = e->next;
    }
    printf("============================================\n");
}
