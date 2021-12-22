#include "libinterface.h"


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