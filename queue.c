#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

queue_t *q_new()
{
    queue_t *q = (queue_t *) malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

void q_free(queue_t *q)
{
    if (!q)
        return;
    list_ele_t *p = q->head;
    while (p) {
        list_ele_t *before;
        before = p;
        p = p->next;
        free(before->value);
        free(before);
    }
    free(q);
}

bool q_insert_head(queue_t *q, char *s)
{
    int length = strlen(s);
    if (!q)
        return false;
    list_ele_t *newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    newh->value = (char *) malloc(sizeof(char) * (length + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, length);
    newh->value[length] = 0;
    newh->next = q->head;
    if (q->size == 0)
        q->tail = newh;
    q->head = newh;
    q->size++;
    return true;
}

bool q_insert_tail(queue_t *q, char *s)
{
    int length = strlen(s);
    if (!q)
        return false;
    list_ele_t *newt = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (!newt)
        return false;
    newt->value = (char *) malloc(sizeof(char) * (length + 1));
    if (!newt->value) {
        free(newt);
        return false;
    }
    strncpy(newt->value, s, length);
    newt->value[length] = 0;
    newt->next = NULL;
    if (q->tail)
        q->tail->next = newt;
    if (q->size == 0)
        q->head = newt;
    q->tail = newt;
    q->size++;
    return true;
}

bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || q->size == 0)
        return false;
    if (sp) {
        strncpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = 0;
    }
    list_ele_t *before = q->head;
    q->head = q->head->next;
    q->size--;
    free(before->value);
    free(before);
    return true;
}

int q_size(queue_t *q)
{
    if (q == NULL || q->head == NULL)
        return 0;
    return q->size;
}

void q_reverse(queue_t *q)
{
    if (q && q->size != 0) {
        list_ele_t *Head, *prev, *current, *Next;
        current = q->head;
        Head = q->head;
        prev = NULL;
        while (current) {
            Next = current->next;
            current->next = prev;
            prev = current;
            current = Next;
        }
        q->head = q->tail;
        q->tail = Head;
    }
}

void q_sort(queue_t *q)
{
    if (!q || q->size == 0)
        return;
    q->tail = q->head = partion(q->head, q->tail);
    while (q->tail->next) {
        q->tail = q->tail->next;
    }
}

list_ele_t *partion(list_ele_t *start, list_ele_t *end)
{
    list_ele_t *slow = start, *fast = start;

    if (start == end)
        return start;
    while (fast != end) {
        if (fast->next->next) {
            fast = fast->next->next;
            slow = slow->next;
        } else {
            if (fast->next)
                fast = fast->next;
        }
    }
    list_ele_t *mid = slow->next;
    slow->next = NULL;
    start = merge(partion(start, slow), partion(mid, fast));
    return start;
}

list_ele_t *merge(list_ele_t *l, list_ele_t *r)
{
    list_ele_t *current = NULL, *newhead = NULL;
    while (l || r) {
        if (!r) {
            current->next = l;
            break;
        } else if (!l) {
            current->next = r;
            break;
        } else if (strcmp(l->value, r->value) <= 0) {
            if (!current)
                newhead = current = l;
            else
                current = current->next = l;
            l = l->next;
        } else {
            if (!current)
                newhead = current = r;
            else
                current = current->next = r;
            r = r->next;
        }
    }
    return newhead;
}