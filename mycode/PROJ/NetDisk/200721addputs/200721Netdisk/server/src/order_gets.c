#include"../include/orders.h"
int order_gets(pReOrder_t pReOrder, char *orderTarget, int userId)
{
    sprintf(pReOrder->retInfo, "user %d's order: gets %s\n", userId, orderTarget);
    return 0;
}
