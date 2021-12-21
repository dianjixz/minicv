#include "imlib_interface.h"
#include "imlib_config.h"
int checkCPU(void)
{
    union w
    {
        int a;
        char b;
    } c;
    c.a = 1;
    return (c.b == 1); // 小端返回TRUE,大端返回FALSE
}




#include <stdio.h>
void log_printf(const char *format,...)
{
    printf(format);
}
