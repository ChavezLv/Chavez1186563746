import ctypes
import struct
from .basic_struct import StructBasic
from .pod_struct import *

class ChanType(object):
    USB_CHAN = 0
    SOCKET_CHAN = 1

#struct  UsbChanCfg {
#	/**
#	* @brief 聚包和拆包功能
#	* 1.  发送聚包 -> 接收拆包：对于用户而言，收到的还是单个NDArray包
#	*  2.  发送聚包 -> 接收不拆包： 发送端只允许聚同一个目的用户的NDArray包，
#	*		接收端收到的是合并在一起的NDArray包，用户自己拆分NDArrays小包，统一释放。		
#	*/
#	int tx_pack_timeout = 0; ///< 单位：ms， 0： 表示单个包直接发送；1 ： 表示等待tx_pack_timeout时间，聚包发送
#	int rx_unpack = 0; ///< 0: 表示接收端不拆包;  1 : 接收端拆包		
#};
class UsbChanCfg(StructBasic):
    _fields_ = [ ("tx_pack_timeout", ctypes.c_int),
                 ("rx_unpack", ctypes.c_int),]

    def __init__(self):
        ctypes.Structure.__init__(self)
        pass

    def __repr__(self):
        _str = 'tx_pack_timeout = %d\n' % (self.tx_pack_timeout)
        _str += 'rx_unpack = %d\n' % (self.rx_unpack)
        return _str

    def to_bytes(self):
        return struct.pack('ii', self.tx_pack_timeout, self.rx_unpack)

    def bytes_sizeof(self):
        return ctypes.sizeof(ctypes.c_int) * 2

#struct SocketChanCfg {
#	std::string url;
#	/* 只允许device 启动server端，port端口需要区别rpc端口 */
#	int port = 0;
#};

class SocketChanCfg(StructBasic):
    _fields_ = [ ("url", StdString),
                 ("port", ctypes.c_int),]

    def __init__(self):
        ctypes.Structure.__init__(self)
        pass

    def __repr__(self):
        _str = 'url = %s\n' % (self.url)
        _str += 'port = %d\n' % (self.port)
        return _str

    def to_bytes(self):
        _bytes = self.url.to_bytes()
        _bytes += struct.pack('i', self.port)
        return _bytes

    def bytes_sizeof(self):
        return ctypes.sizeof(ctypes.c_int) + len(self.url)


#struct P2PChanCfg {
#		int p2ptype = USB_CHAN;
#		int block_max_size = 4 << 20;
#		int queue_size = 16; ///< tensor 发送线程的队列深度
#		SocketChanCfg socket_cfg;  
#		UsbChanCfg usb_cfg;
#	};/* P2PChan配置参数 */
class P2PChanCfg(StructBasic):
    #_pack_ = 1
    _fields_ = [ ("p2ptype", ctypes.c_int),
                 ("block_max_size", ctypes.c_int),
                 ("queue_size", ctypes.c_int),
                 ("need_check_crc", ctypes.c_int),
                 ("crc_use_tensor_data", ctypes.c_int),
                 ("socket_cfg", SocketChanCfg),
                 ("usb_cfg", UsbChanCfg),]

    def __init__(self):
        self.p2ptype = ChanType.SOCKET_CHAN
        self.block_max_size = 4 << 20
        self.queue_size = 16
        self.need_check_crc = 1
        self.crc_use_tensor_data = 0
        ctypes.Structure.__init__(self)
        pass

    def __repr__(self):
        _str = 'p2ptype = %d\n' % (self.p2ptype)
        _str += 'block_max_size = %d\n' % (self.block_max_size)
        _str += 'queue_size = %d\n' % (self.queue_size)
        _str += 'need_check_crc = %d\n' % (self.need_check_crc)
        _str += 'crc_use_tensor_data = %d\n' % (self.crc_use_tensor_data)
        _str += 'socket_cfg = %s\n' % (self.socket_cfg.__repr__())
        _str += 'usb_cfg = %d\n' % (self.usb_cfg.__repr__())
        return _str

    def to_bytes(self):
        return (struct.pack('5i', self.p2ptype, self.block_max_size, self.queue_size, self.need_check_crc, self.crc_use_tensor_data)
                 + self.socket_cfg.to_bytes() + self.usb_cfg.to_bytes())

    def bytes_sizeof(self):
        return ctypes.sizeof(ctypes.c_int) * 5 + self.socket_cfg.bytes_sizeof() + self.usb_cfg.bytes_sizeof()

