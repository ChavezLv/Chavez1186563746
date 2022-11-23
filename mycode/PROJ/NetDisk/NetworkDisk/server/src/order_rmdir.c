#include"../include/orders.h"
int order_rmdir(pReOrder_t pReOrder, char *orderTarget, int userId)
{
    sprintf(pReOrder->retInfo, "user %d's order: rmdir %s\n", userId, orderTarget);
    return 0;
}
