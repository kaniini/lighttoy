#include <stdio.h>
#include <math.h>
#include <time.h>
#include <libbulb/libbulb.h>

struct libbulb_group group;

int
main(int argc, const char *argv[])
{
    struct libbulb_node *node;
    int i = 0;

    (void) argc;
    (void) argv;

    srand(time(NULL));
    libbulb_group_discover(&group);

    LIBBULB_FOREACH_LIST_ENTRY(group.lights.head, node)
    {
        struct libbulb_light *light = node->data;

        printf("Light %d, address %0.2x%0.2x.%0.2x%0.2x.%0.2x%0.2x.%0.2x%0.2x:\n",
               ++i, light->address[0], light->address[1], light->address[2], light->address[3],
               light->address[4], light->address[5], light->address[6], light->address[7]);
        printf("    Hue: %f\n", light->color.hue);
        printf("    Sat: %f\n", light->color.saturation);
        printf("    Val: %f\n", light->color.value);
        printf(" Kelvin: %f\n", light->color.kelvin);
        printf("  Power: %s\n", light->powered ? "ON" : "OFF");

        libbulb_light_set_color(light, (struct libbulb_color){
                .hue = rand() % 360,
                .saturation = (rand() % 100) * 0.01,
                .value = (rand() % 100) * 0.01,
                .kelvin = (rand() % 10000),
        });
    }
}
