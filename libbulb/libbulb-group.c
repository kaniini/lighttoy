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

#include <libbulb/libbulb.h>
#include <libbulb/protocol.h>

bool
libbulb_group_discover(struct libbulb_group *group)
{
    return false;
}
