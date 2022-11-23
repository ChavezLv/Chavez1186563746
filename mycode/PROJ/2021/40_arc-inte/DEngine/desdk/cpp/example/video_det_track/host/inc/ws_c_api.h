#ifndef IF_WS_C_API_H
#define IF_WS_C_API_H

#ifdef _WIN32
#define IF_WS_SDK_CALL __stdcall
#ifdef  IF_SDK_EXPORTS
#define IF_WS_SDK_API __declspec(dllexport)
#else
#define IF_WS_SDK_API __declspec(dllimport)
#endif
#else
#define IF_WS_SDK_CALL 
#define IF_WS_SDK_API __attribute ((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*if_ws_on_open_cb_t)(int session_id, void *usrdata);
typedef void (*if_ws_on_fail_cb_t)(int session_id, void *usrdata);
typedef void (*if_ws_on_close_cb_t)(int session_id, void *usrdata);
typedef void (*if_ws_on_message_cb_t)(int session_id, const void* message, int len, void *usrdata);

typedef enum {
  IF_WS_OK                 =  0, ///< 正常
  IF_WS_ERR_RUNTIME        = -1,   ///< 运行错误
  IF_WS_ERR_INVALID_ARG    = -2,   ///< 无效参数
  IF_WS_ERR_INVALID_HANDLE = -3,   ///< 无效句柄
} if_ws_result_t;


IF_WS_SDK_API if_ws_result_t IF_WS_SDK_CALL 
if_ws_client_connect(const char* uri,   const char* con_name, if_ws_on_open_cb_t open_cb, 
    if_ws_on_fail_cb_t fail_cb, if_ws_on_message_cb_t msg_cb, if_ws_on_close_cb_t close_cb);

IF_WS_SDK_API if_ws_result_t IF_WS_SDK_CALL 
if_ws_client_send(int session_id, void const* data, int len);

IF_WS_SDK_API if_ws_result_t IF_WS_SDK_CALL 
if_ws_server_start(int port, if_ws_on_open_cb_t open_cb, 
    if_ws_on_message_cb_t msg_cb, if_ws_on_close_cb_t close_cb);

IF_WS_SDK_API if_ws_result_t IF_WS_SDK_CALL 
if_ws_server_send(int session_id, void const* data, int len);

#ifdef __cplusplus
}
#endif
#endif