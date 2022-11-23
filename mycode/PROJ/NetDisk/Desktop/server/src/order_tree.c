#include"../include/orders.h"
int order_tree(pReOrder_t pReOrder, char *orderTarget, int userId)
{
    sprintf(pReOrder->retInfo, "user %d's order: tree %s\n", userId, orderTarget);
    return 0;
}
