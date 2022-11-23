#include"../include/orders.h"
int order_puts(pReOrder_t pReOrder, char *orderTarget, int userId)
{
    sprintf(pReOrder->retInfo, "user %d's order: puts %s\n", userId, orderTarget);
    return 0;
}
