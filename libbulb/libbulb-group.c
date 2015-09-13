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

#include <assert.h>

#include <libbulb/libbulb.h>
#include <libbulb/protocol.h>

static int
libbulb_group_sockopen(bool broadcast)
{
    int sock, yes = 1;
    struct sockaddr_in saddr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        return -1;

    memset(&saddr, 0, sizeof saddr);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = 0;

    if (bind(sock, (struct sockaddr *) &saddr, sizeof saddr) < 0)
    {
        close(sock);
        return -1;
    }

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 200000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

    if (broadcast)
    {
        setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &yes, sizeof yes);
    }

    return sock;
}

static int
libbulb_group_send(int sock, lx_protocol_t header, struct sockaddr_in to_addr)
{
    return sendto(sock, &header, header.size, 0, (struct sockaddr *) &to_addr, sizeof to_addr);
}

bool
libbulb_group_discover(struct libbulb_group *group)
{
    int sock;
    lx_protocol_t header, msg;
    struct sockaddr_in saddr;
    struct sockaddr_in raddr;
    socklen_t addrlen = sizeof(raddr);
    int recvlen;
    int counter = 0, attempts = 10;

    assert(group);

    memset((char *) &saddr, 0, sizeof saddr);

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(56700);
    saddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    header.protocol = 1024;
    header.addressable = 1;
    header.tagged = 1;
    header.origin = 0;
    header.source = 1;

    for (int i = 0; i < 8; i++)
        header.target[i] = 0;

    header.ack_required = 0;
    header.res_required = 0;
    header.sequence = 0;
    header.reserved16 = 0;

    for(int i = 0; i < 6; i++) {
        header.reserved[i] = 0;
    }

    header.type = LX_PROTOCOL_DEVICE_GET_SERVICE;
    header.size = offsetof(lx_protocol_t, payload);

    sock = libbulb_group_sockopen(true);

    libbulb_group_send(sock, header, saddr);
    while (--attempts > 0) {
        recvlen = recvfrom(sock, &msg, sizeof msg, 0, (struct sockaddr *) &raddr, &addrlen);
        if (recvlen < 0 && counter > 0)
            break;
        if (recvlen == 0)
            continue;
        if (msg.type == LX_PROTOCOL_DEVICE_STATE_SERVICE)
        {
            struct libbulb_light *light = calloc(sizeof(*light), 1);

            counter++;

            for (int i = 0; i < 8; i++)
                light->address[i] = msg.target[i];

            libbulb_node_insert(&light->gnode, light, &group->lights);
        }
    }

    close(sock);

    return counter > 0;
}
