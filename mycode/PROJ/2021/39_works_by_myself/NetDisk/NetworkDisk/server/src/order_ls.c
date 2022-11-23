#include"../include/orders.h"
int order_ls(pReOrder_t pReOrder, char *orderTarget, int userId)
{
    sprintf(pReOrder->retInfo, "user %d's order: ls %s\n", userId, orderTarget);
    return 0;
}

