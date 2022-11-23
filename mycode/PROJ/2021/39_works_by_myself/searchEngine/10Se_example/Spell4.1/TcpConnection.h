///
/// @file    TcpConnection.h
/// @author  lemon(haohb13@gmail.com)
/// @date    2019-05-07 16:44:49
///

#ifndef __WD_TCPCONNECTION_H__
#define __WD_TCPCONNECTION_H__

#include "Socket.h"
#include "InetAddress.h"
#include "SocketIO.h"
#include "Noncopyable.h"

#include <string>
#include <functional>
#include <memory>

using std::shared_ptr;
using std::string;
using std::function;
namespace wd
{
class EventLoop;
class TcpConnection;
using TcpConnectionPtr=shared_ptr<TcpConnection>;
using TcpCallBack=function<void(const TcpConnectionPtr&)>;
class TcpConnection
    : Noncopyable
    ,public std::enable_shared_from_this<TcpConnection>
    {
    public:
        TcpConnection(int fd,EventLoop*);
        ~TcpConnection();

        string receive();
        void send(const string & msg);
        void sendInloop(const string&);
        
        int getPeerfd(){return _sock.fd();}
        string toString() const;
        void shutdown();

        void setNewConnectionCallBack(const TcpCallBack&cb){
            _newConnection=cb;
        }
        void setMsgCallBack(const TcpCallBack&cb){
            _msg=cb;
        }
        void setCloseCallBack(const TcpCallBack&cb){
            _Close=cb;
        }

        void handleNewConnectionCallBack(){
            if(_newConnection){
                _newConnection(shared_from_this());
            }
        }
        void handleMessageCallBack(){
            if(_msg){
                _msg(shared_from_this());
            }
        }
        void handleCloseCallBack(){
            if(_Close){
                _Close(shared_from_this());
            }
        }
    private:
        InetAddress getLocalAddr(int fd);
        InetAddress getPeerAddr(int fd);
    private:
        EventLoop* _eventloop;
        
        TcpCallBack _newConnection;
        TcpCallBack _msg;
        TcpCallBack _Close;

        Socket _sock;
        SocketIO _socketIo;
        InetAddress _localAddr;
        InetAddress _peerAddr;
        bool _isShutdwonWrite;
    };

}//end of namespace wd
#endif
