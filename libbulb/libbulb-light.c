/*
 * libbulb-group.c
 * libbulb - group management.
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

static bool
libbulb_light_match_address(uint8_t *a, uint8_t *b)
{
    return !memcmp(a, b, 8);
}

static int
libbulb_light_send(int sock, lx_protocol_t header, struct sockaddr_in to_addr)
{
    return libbulb_send(sock, &header, header.size, to_addr);
}

bool
libbulb_light_get_state(struct libbulb_light *light)
{
    int sock;
    int recvlen;
    struct sockaddr_in raddr;
    socklen_t addrlen = sizeof raddr;

    sock = libbulb_sockopen(false);

    lx_protocol_t header;
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
    header.reserved64 = 0;
    for(int i = 0; i < 6; i++) {
        header.reserved[i] = 0;
    }
    header.type = LX_PROTOCOL_LIGHT_GET;
    header.size = offsetof(lx_protocol_t, payload);
    lx_protocol_t msg;

    int attempts = 5;
    while (--attempts) {
        libbulb_light_send(sock, header, light->addr);

        recvlen = recvfrom(sock, &msg, sizeof msg, 0, (struct sockaddr *) &raddr, &addrlen);
        if (recvlen < 0) {
            perror("recvfrom");
            break;
        }
        if (recvlen == 0)
            continue;

        if (msg.type == LX_PROTOCOL_LIGHT_STATE && libbulb_light_match_address(msg.target, light->address))
        {
            memset(light->label, '\0', sizeof(light->label));
            memcpy(light->label, msg.payload.lx_protocol_light_state.label, 32);

            light->color.hue        = (float)(msg.payload.lx_protocol_light_state.color.hue)/0xFFFF * 360;
            light->color.saturation = (float)msg.payload.lx_protocol_light_state.color.saturation/0xFFFF;
            light->color.value      = (float)msg.payload.lx_protocol_light_state.color.brightness/0xFFFF;
            light->color.kelvin     = msg.payload.lx_protocol_light_state.color.kelvin;
            light->powered          = msg.payload.lx_protocol_light_state.power;

            break;
        }
    }

    close(sock);

    return attempts > 0;
}

bool
libbulb_light_set_powered(struct libbulb_light *light, bool powered)
{
    int sock;

    sock = libbulb_sockopen(false);

    lx_protocol_t header;
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
    header.reserved64 = 0;
    for(int i = 0; i < 6; i++) {
        header.reserved[i] = 0;
    }
    header.type = LX_PROTOCOL_LIGHT_SET_POWER;
    header.payload.lx_protocol_light_set_power.level = powered ? 0xFFFF : 0;
    header.payload.lx_protocol_light_set_power.duration = 0;
    header.size = lx_protocol_sizeof(lx_protocol_light_set_power_t);

    libbulb_light_send(sock, header, light->addr);

    close(sock);

    light->powered = powered;
    return light->powered;
}
