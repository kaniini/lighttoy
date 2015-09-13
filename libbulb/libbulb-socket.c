/*
 * libbulb-socket.c
 * libbulb - socket functions.
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

int
libbulb_sockopen(bool broadcast)
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

int
libbulb_send(int sock, void *buf, size_t size, struct sockaddr_in to_addr)
{
    return sendto(sock, buf, size, 0, (struct sockaddr *) &to_addr, sizeof to_addr);
}
