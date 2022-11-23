#include"../include/orders.h"
int order_pwd(pReOrder_t pReOrder, int userId)
{
    sprintf(pReOrder->retInfo, "user %d's order: pwd \n", userId);
    return 0;
}
