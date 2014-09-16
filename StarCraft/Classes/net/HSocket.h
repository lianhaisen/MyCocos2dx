//
//  HSocket.h
//  Coc
//  封装Socket线程
//  Created by Hector on 13-10-26.
//
//

#ifndef __Coc__HSocket__
#define __Coc__HSocket__

#include <iostream>
#include <string.h>
#include <queue>

#include "cocos2d.h"
#include "ODSocket.h"
#include "ByteBuffer.h"

using namespace std;

class MessageQueue;

//客户端状态
enum SocketClientStatus {
    SocketClient_WAIT_CONNECT, // 等待连接
    SocketClient_OK,           // 正常
    SocketClient_DESTROY       // 丢失
};

enum MsgStatus
{
	MsgStatus_OK=0,				// 正常状态
	MsgStatus_Connect,			// 连接成功
	MsgStatus_Close,			// 客户端自己关闭

	MsgStatus_Error_UNKNOW = -1,// 异常中断
	MsgStatus_Error_System = -2,// 系统出错
	MsgStatus_Error_Timeout= -3,// 超时
	MsgStatus_Error_Buffout= -4,// 缓冲区溢出
	MsgStatus_Error_Read   = -5,// 读取数据异常
	MsgStatus_Error_ServerClose = -6,// 服务端关闭
	MsgStatus_Error_Send   = -7,     // 发送异常
	MsgStatus_Error_SendBuff= -8,    // 发送包异常，可能太大,或者网络太慢缓冲已满
};

typedef struct Message {
    char* data;
    int sz;
	int status;
} *PMessage;
class Netcall;

class SocketClient
{
public:
    ODSocket* m_socket;
  
    //客户端状态
    SocketClientStatus m_Status;
    
    queue<Message*> m_sendMessageQueue; //消息发送队列
    std::mutex m_sendqueue_mutex;//发送队列 锁,访问时加锁
    
	queue<Message*> m_receivedMessageQueue;//消息接收队列
    std::mutex m_recvqueue_mutex;//接收队列 锁,访问时加锁
	
    
    //发送和接收缓冲区，发送缓冲区满的时候，会断开连接，并提示信号不好
	ByteBuffer m_cbRecvBuf;
	ByteBuffer m_cbSendBuf;
    
    //send thread
	bool m_bThreadSendCreated; // 是否创建线程
    std::thread *pthread_t_send;
    
    //receive thread
	bool m_bThreadRecvCreated; // 是否创建接收线程
	std::thread *pthread_t_receive;

    //用来控制线程运行的锁，如计时，唤醒等
    std::mutex m_thread_cond_mutex;//互斥锁
	std::condition_variable m_threadCond;
    
    static int bytesToInt(byte* data);
	static byte* intToByte(int i);
public:
	static SocketClient* connect(const char* ip, unsigned short port);
    SocketClient(const char* ip, unsigned short port);

    ~SocketClient();
    void sendMessage(Message* msg);//发送消息
    void stop();
    bool start();  //create send thread
    void reConnect();
	void setTag(const char* tag){ m_tag=tag;}
	string getTag(){ return m_tag;}

	Message* popReceivedMessage();
	void ReceiveStatus(MsgStatus s);
private:
    void threadReceiveMessage(); //
	void threadSendMessage();
    void clearRecvQueue();
    void clearSendQueue();

	string m_ip;
	string m_tag;
	int m_port;
};

#endif /* defined(__Coc__HSocket__) */
