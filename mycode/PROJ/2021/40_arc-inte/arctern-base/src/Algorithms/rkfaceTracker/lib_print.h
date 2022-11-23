#ifndef _LIB_PRINT_H
#define _LIB_PRINT_H

#define _ALGO_TRACE

#define LIB_ALGO_PRINTF(Msg, ...)\
do\
{\
    fprintf(stderr,"[Func]:%s [Line]:%d [Info]:"Msg, __FUNCTION__, __LINE__, ## __VA_ARGS__);\
}while(0)

#define LIB_ALGO_TRACE(Msg, ...)\
do\
{\
	LIB_ALGO_PRINTF(Msg, ##__VA_ARGS__);\
}while(0)

#define LIB_ALGO_CHECK_EXPR_RET(Expr,Ret,Msg, ...)\
do{\
	if(Expr)\
	{\
		LIB_ALGO_TRACE(Msg, ##__VA_ARGS__);\
		return Ret;\
	}\
}while(0)

#define LIB_ALGO_CHECK_EXPR_NO_RET(Expr,Msg, ...)\
do{\
    if(Expr)\
    {\
        LIB_ALGO_TRACE(Msg, ##__VA_ARGS__);\
    }\
}while(0)


#endif
