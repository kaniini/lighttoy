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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

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
    struct libbulb_node cnode;
    char label[LIBBULB_LABEL_LEN];
    uint8_t address[LIBBULB_ADDRESS_LEN];
    struct sockaddr_in addr;
    struct libbulb_color color;
    bool powered;
};

#endif
