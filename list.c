// list/list.c
// 
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

list_t *list_alloc(void) { 
  list_t *mylist =  (list_t *) malloc(sizeof(list_t)); 
  if (mylist == NULL) {
    return NULL;
  }
  mylist->head = NULL;
  return mylist;
}

void list_free(list_t *l) {
  if (l == NULL) return;
  node_t *cur = l->head;
  while (cur != NULL) {
    node_t *next = cur->next;
    free(cur);
    cur = next;
  }
  free(l);
}

void list_print(list_t *l) {
    if (l == NULL) {
        printf("NULL\n");
        return;
    }
    node_t *cur = l->head;
    while (cur != NULL) {
        printf("%d->", cur->value);
        cur = cur->next;
    }
    printf("NULL\n");
}


char *listToString(list_t *l) {
    if (l == NULL) {
        char *empty = (char *)malloc(5);
        if (!empty) return NULL;
        strcpy(empty, "NULL");
        return empty;
    }

    size_t cap = 64, len = 0;
    char *buf = (char *)malloc(cap);
    if (!buf) return NULL;

    node_t *curr = l->head;
    while (curr) {
        char tbuf[32];
        int n = snprintf(tbuf, sizeof tbuf, "%d->", curr->value);
        if (n < 0) { free(buf); return NULL; }

        if (len + (size_t)n + 5 >= cap) {
            while (len + (size_t)n + 5 >= cap) cap *= 2;
            char *nb = (char *)realloc(buf, cap);
            if (!nb) { free(buf); return NULL; }
            buf = nb;
        }
        memcpy(buf + len, tbuf, (size_t)n);
        len += (size_t)n;
        curr = curr->next;
    }

    const char *tail = "NULL";
    size_t tailn = 4;
    if (len + tailn + 1 >= cap) {
        size_t newcap = len + tailn + 1;
        char *nb = (char *)realloc(buf, newcap);
        if (!nb) { free(buf); return NULL; }
        buf = nb; cap = newcap;
    }
    memcpy(buf + len, tail, tailn);
    len += tailn;
    buf[len] = '\0';
    return buf;
}


int list_length(list_t *l) {
  if (l == NULL) return 0;
  int count = 0;
  node_t *cur = l->head;
  while (cur != NULL) {
    count++;
    cur = cur->next;
      
  }
  return count;
}

void list_add_to_back(list_t *l, elem value) {
    if (l == NULL) return;

    node_t *n = getNode(value);
    if (n == NULL) return;

    if (l->head == NULL) {
        l->head = n;
        return;
    }

    node_t *cur = l->head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = n;
}

void list_add_to_front(list_t *l, elem value) {
    if (l == NULL) return;
    node_t *cur_node = getNode(value);
    if (cur_node == NULL) return;
    cur_node->next = l->head;
    l->head = cur_node;
}


node_t * getNode(elem value) {
  node_t *mynode = (node_t *)malloc(sizeof(node_t));
  if (mynode == NULL) {
    return NULL;
  }

  mynode->value = value;
  mynode->next = NULL;
  return mynode;
}

void list_add_at_index(list_t *l, elem value, int index) {
    if (l == NULL) return;

    int len = list_length(l);
    if (index <= 1 || l->head == NULL) {
        list_add_to_front(l, value);
        return;
    }
    if (index > len) {
        list_add_to_back(l, value);
        return;
    }

    node_t *prev = l->head;
    int i;
    for (i = 1; i < index - 1 && prev != NULL; i++) {
        prev = prev->next;
    }
    if (prev == NULL) return;

    node_t *n = getNode(value);
    if (!n) return;
    n->next = prev->next;
    prev->next = n;
}








elem list_remove_from_back(list_t *l) {
    if (l == NULL || l->head == NULL) return -1;
    node_t *cur = l->head;
    node_t *prev = NULL;
    while (cur->next != NULL) {
        prev = cur;
        cur = cur->next;
    }
    elem v = cur->value;
    if (prev != NULL) prev->next = NULL;
    else l->head = NULL;
    free(cur);
    return v;
}

elem list_remove_from_front(list_t *l) {
    if (l == NULL || l->head == NULL) return -1;
    node_t *first = l->head;
    elem v = first->value;
    l->head = first->next;
    free(first);
    return v;
}

elem list_remove_at_index(list_t *l, int index) {
    if (l == NULL || l->head == NULL) return -1;
    if (index <= 0) return -1;
    if (index == 1) return list_remove_from_front(l);

    int len = list_length(l);
    if (index > len) return -1;

    node_t *prev = l->head;
    int i;
    for (i = 1; i < index - 1 && prev->next != NULL; i++) {
        prev = prev->next;
    }
    node_t *cur = prev->next;
    if (cur == NULL) return -1;

    elem v = cur->value;
    prev->next = cur->next;
    free(cur);
    return v;
}







bool list_is_in(list_t *l, elem value) {
    if (l == NULL) return false;
    node_t *cur = l->head;
    while (cur != NULL) {
        if (cur->value == value) return true;
        cur = cur->next;
    }
    return false;
}

elem list_get_elem_at(list_t *l, int index) {
    if (l == NULL || index < 1) return -1;
    int i = 1;
    node_t *cur = l->head;
    while (cur != NULL && i < index) {
        cur = cur->next;
        i++;
    }
    if (cur == NULL) return -1;
    return cur->value;
}

int list_get_index_of(list_t *l, elem value) {
    if (l == NULL) return -1;
    int i = 1;
    node_t *cur = l->head;
    while (cur != NULL) {
        if (cur->value == value) return i;
        cur = cur->next;
        i++;
    }
    return -1;
}


