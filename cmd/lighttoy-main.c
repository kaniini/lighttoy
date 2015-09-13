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

#include <getopt.h>
#include <libbulb/libbulb.h>

struct libbulb_group root_group;

#define PARAM_LIGHT		0
#define PARAM_TOGGLE		1
#define PARAM_ON		2
#define PARAM_OFF		3
#define PARAM_PRINT             4
#define PARAM_HUE               5
#define PARAM_SATURATION        6
#define PARAM_VALUE             7
#define PARAM_KELVIN            8
#define PARAM_SETCOLOR          9
#define PARAM_SIZE		10

typedef void (*callback_func)(struct libbulb_light *light);

static void
apply_func_to_light(struct libbulb_light *light, callback_func func)
{
    func(light);
}

static void
apply_func_to_group(struct libbulb_group *group, callback_func func)
{
    struct libbulb_node *n;

    LIBBULB_FOREACH_LIST_ENTRY(group->lights.head, n)
        apply_func_to_light(n->data, func);
}

static void func_toggle(struct libbulb_light *light) { libbulb_light_set_powered(light, !light->powered); }
static void func_on(struct libbulb_light *light)     { libbulb_light_set_powered(light, true); }
static void func_off(struct libbulb_light *light)    { libbulb_light_set_powered(light, false); }

static void func_print(struct libbulb_light *light)
{
    printf("Light '%s' [%0.2x%0.2x.%0.2x%0.2x.%0.2x%0.2x.%0.2x%0.2x]:\n", light->label,
        light->address[0], light->address[1], light->address[2], light->address[3],
        light->address[4], light->address[5], light->address[6], light->address[7]);
    printf("         Hue: %f\n", light->color.hue);
    printf("  Saturation: %f\n", light->color.saturation);
    printf("       Value: %f\n", light->color.value);
    printf("      Kelvin: %f\n", light->color.kelvin);
    printf("       State: %s\n", light->powered ? "ON" : "OFF");
}

static void func_hue(struct libbulb_light *light)        { light->color.hue = atof(optarg); }
static void func_saturation(struct libbulb_light *light) { light->color.saturation = atof(optarg); }
static void func_value(struct libbulb_light *light)      { light->color.value = atof(optarg); }
static void func_kelvin(struct libbulb_light *light)     { light->color.kelvin = atof(optarg); }
static void func_setcolor(struct libbulb_light *light) {
    struct libbulb_color col = light->color;
    libbulb_light_set_color(light, col);
}

int
main(int argc, char *argv[])
{
    int ret;

    static struct option options[] = {
        {"light", required_argument, NULL, PARAM_LIGHT},
        {"toggle", no_argument,      NULL, PARAM_TOGGLE},
        {"on", no_argument,          NULL, PARAM_ON},
        {"off", no_argument,         NULL, PARAM_OFF},
        {"info", no_argument,        NULL, PARAM_PRINT},
        {"hue", required_argument,   NULL, PARAM_HUE},
        {"saturation", required_argument, NULL, PARAM_SATURATION},
        {"value", required_argument, NULL, PARAM_VALUE},
        {"kelvin", required_argument, NULL, PARAM_KELVIN},
        {"setcolor", no_argument,    NULL, PARAM_SETCOLOR},
    };
    static callback_func funcs[PARAM_SIZE] = {
        [PARAM_ON] = func_on,
        [PARAM_OFF] = func_off,
        [PARAM_TOGGLE] = func_toggle,
        [PARAM_PRINT] = func_print,
        [PARAM_HUE] = func_hue,
        [PARAM_SATURATION] = func_saturation,
        [PARAM_VALUE] = func_value,
        [PARAM_KELVIN] = func_kelvin,
        [PARAM_SETCOLOR] = func_setcolor,
    };

    libbulb_group_discover(&root_group);

    while ((ret = getopt_long(argc, argv, "", options, NULL)) != -1)
    {
        if (ret > PARAM_SIZE)
            continue;
        apply_func_to_group(&root_group, funcs[ret]);
    }

    return 0;
}
