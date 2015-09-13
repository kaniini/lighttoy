/*
 * iter.h
 * Linked lists and iterators.
 *
 * Copyright (c) 2015 William Pitcock <nenolod@dereferenced.org>.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * This software is provided 'as is' and without any warranty, express or
 * implied.  In no event shall the authors be liable for any damages arising
 * from the use of this software.
 */

#ifndef LIBBULB__ITER_H
#define LIBBULB__ITER_H

struct libbulb_node {
    struct libbulb_node *prev, *next;
    void *data;
};

struct libbulb_list {
    struct libbulb_node *head, *tail;
};

#define LIBBULB_LIST_INITIALIZER		{ NULL, NULL }

static inline void
libbulb_node_insert(struct libbulb_node *node, void *data, struct libbulb_list *list)
{
    struct libbulb_node *tnode;

    node->data = data;

    if (list->head == NULL)
    {
        list->head = node;
        list->tail = node;
        return;
    }

    tnode = list->head;

    node->next = tnode;
    tnode->prev = node;

    list->head = node;
}

static inline void
libbulb_node_insert_tail(struct libbulb_node *node, void *data, struct libbulb_list *list)
{
    struct libbulb_node *tnode;

    node->data = data;

    if (list->head == NULL)
    {
        list->head = node;
        list->tail = node;
        return;
    }

    tnode = list->tail;

    node->prev = tnode;
    tnode->next = node;

    list->tail = node;
}

static inline void
libbulb_node_delete(struct libbulb_node *node, struct libbulb_list *list)
{
    if (node->prev == NULL)
        list->head = node->next;
    else
        node->prev->next = node->next;

    if (node->next == NULL)
        list->tail = node->prev;
    else
        node->next->prev = node->prev;
}

#endif
