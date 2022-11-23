#include"../include/orders.h"
int order_mkdir(pReOrder_t pReOrder, char *orderTarget, int userId)
{
    sprintf(pReOrder->retInfo, "user %d's order: mkdir %s\n", userId, orderTarget);
    return 0;
}

