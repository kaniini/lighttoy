/*
 * libbulb.h
 * libbulb - main header.  toplevel management.
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

#ifndef LIBBULB__LIBBULB_H
#define LIBBULB__LIBBULB_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <libbulb/iter.h>

#define LIBBULB_ADDRESS_LEN (8)
#define LIBBULB_LABEL_LEN (255+1)

struct libbulb_color {
    float hue;
    float saturation;
    float value;
    float kelvin;
};

struct libbulb_group {
    struct libbulb_list lights;
};

struct libbulb_light {
    struct libbulb_node gnode;
    char label[LIBBULB_LABEL_LEN];
    uint8_t address[LIBBULB_ADDRESS_LEN];
    struct sockaddr_in addr;
    struct libbulb_color color;
    bool powered;
};

/*
 * \group util functions
 */

/*
 * \func libbulb_sockopen
 * \return socket fd
 */
extern int libbulb_sockopen(bool broadcast);
extern int libbulb_send(int sock, void *msg, size_t size, struct sockaddr_in saddr);

/*
 * \group group functions
 */

/*
 * \func libbulb_group_discover
 * \return true if peers were found, false otherwise/error
 */
extern bool libbulb_group_discover(struct libbulb_group *group);

/*
 * \group light functions
 */
extern bool libbulb_light_get_state(struct libbulb_light *light);

#endif
