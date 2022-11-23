#include"../include/orders.h"
int order_cd(pReOrder_t pReOrder, char *orderTarget, int userId)
{
    sprintf(pReOrder->retInfo, "user %d's order: cd %s\n", userId, orderTarget);
    return 0;
}

