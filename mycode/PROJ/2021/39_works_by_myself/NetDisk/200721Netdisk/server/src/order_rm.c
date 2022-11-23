#include"../include/orders.h"
int order_rm(pReOrder_t pReOrder, char *orderTarget, int userId)
{
    sprintf(pReOrder->retInfo, "user %d's order: rm %s\n", userId, orderTarget);
    return 0;
}
