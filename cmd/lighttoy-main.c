/*
 * lighttoy-main.c
 * lighttoy - main()
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

struct libbulb_group root_group;

int
main(int argc, const char *argv[])
{
    libbulb_group_discover(&root_group);

    return 0;
}
