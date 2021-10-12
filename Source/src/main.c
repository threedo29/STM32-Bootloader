#include "main.h"

int main(void)
{
    chip_config();
    peripheral_init();

    app_main();
}
