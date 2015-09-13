#include <stdio.h>
#include <libbulb/libbulb.h>

struct libbulb_group group;

int
main(int argc, const char *argv[])
{
    struct libbulb_node *node;
    int i = 0;

    (void) argc;
    (void) argv;

    libbulb_group_discover(&group);

    LIBBULB_FOREACH_LIST_ENTRY(group.lights.head, node)
    {
        struct libbulb_light *light = node->data;

        printf("Light %d, address %0.2x%0.2x.%0.2x%0.2x.%0.2x%0.2x.%0.2x%0.2x\n",
               ++i, light->address[0], light->address[1], light->address[2], light->address[3],
               light->address[4], light->address[5], light->address[6], light->address[7]);
    }
}
