//
//  HSocket.cpp
//  Coc
//
//  Created by Hector on 13-10-26.
//
//

#include "HSocket.h"
#include "ByteBuffer.h"
#include "AutoReleasePool.h"

using namespace cocos2d;

SocketClient::SocketClient(const char* ip, unsigned short port):
m_Status(SocketClient_WAIT_CONNECT),
m_cbRecvBuf(1024*60),m_cbSendBuf(1024*60)
{
    m_ip = ip;
    m_port =port;
	
    m_Status = SocketClient_WAIT_CONNECT;
    
    m_socket = new ODSocket(); //新建对象
    m_socket->Init();
	m_bThreadRecvCreated = false;
	m_bThreadSendCreated = false;
}


SocketClient::~SocketClient()
{
    this->stop();
    CC_SAFE_DELETE(m_socket);
}

SocketClient* SocketClient::connect( const char* ip, unsigned short port)
{
	SocketClient *c = nullptr;
	c = new SocketClient(ip, port);
	if (c->start()==false)
	{
		delete c;
		return NULL;
	}

	return c;
}


void SocketClient::reConnect()
{
    m_thread_cond_mutex.lock();
    m_socket->Close(); //关闭之前的套接字
    m_Status=SocketClient_WAIT_CONNECT;
    m_bThreadRecvCreated = false;
	m_bThreadSendCreated = false;
	m_thread_cond_mutex.unlock();
    this->start();
}

void SocketClient::threadReceiveMessage()
{
	fd_set fdRead;
	
	struct timeval	aTime;
    
	//最大多少秒，连接上收不到数据就提示用户，重新登录
	int maxIdleTimeInSeconds = 60*3;
	
	//最大多少秒，连接上收不到数据就提示用户，选择重连
	int hint2TimeInSeconds = 60;
	
	//多长时间没有收到任何数据，提示用户
	int hintTimeInSeconds = 30;
	
	struct timeval lastHintUserTime;
	struct timeval lastReceiveDataTime;
	struct timeval now;
	
	gettimeofday(&lastReceiveDataTime, NULL);
	lastHintUserTime = lastReceiveDataTime;
	
	SocketClient* this_ = this;
	
	ByteBuffer* recvBuff =new ByteBuffer(1024);
    recvBuff= &this_->m_cbRecvBuf;
    
	while (this_->m_Status != SocketClient_DESTROY)
	{
		if( this_->m_Status != SocketClient_OK) //挂起一段时间，等待连接
        {
			std::chrono::milliseconds dura( 1000 );
			std::this_thread::sleep_for( dura );
			continue;
		}
        
        //每1s查看是否有数据可读
		FD_ZERO(&fdRead);
		FD_SET(SOCKET(*this_->m_socket),&fdRead);
		aTime.tv_sec = 1;
		aTime.tv_usec = 0;
		int ret = select(((SOCKET)*this_->m_socket)+1,&fdRead,NULL,NULL,&aTime);
        
		if (ret == -1 ) // -1 mean error
		{
			if(errno == EINTR)
            {
				log("服务器中断，do nothing");
				this_->ReceiveStatus(MsgStatus_Error_UNKNOW);
			}
            else //系统出错
            {
				this_->m_Status = SocketClient_DESTROY;
                log("select error, receive thread stop! errno=%d, address=%p\n",errno,this_);
				this_->ReceiveStatus(MsgStatus_Error_System);
				return;
			}
		}
		else if(ret==0) // 0 mean connect timeout
		{
            
			gettimeofday(&now, NULL);
			
            if(now.tv_sec - lastReceiveDataTime.tv_sec > maxIdleTimeInSeconds && now.tv_sec - lastHintUserTime.tv_sec > hintTimeInSeconds)
            {
                lastHintUserTime = now;
                this_->clearRecvQueue();
                log("SocketClient::threadReceiveMessage:timeout");//您的网络已经出问题了,请重新登录~
				this_->ReceiveStatus(MsgStatus_Error_Timeout);
            }
            else
            {
                lastHintUserTime = now;
                lastReceiveDataTime= now;
            }
		}
		else if (ret > 0) //find have data to read
		{
			if (FD_ISSET((SOCKET)*this_->m_socket,&fdRead))
			{
				int iRetCode = 0;
                //将数据读入缓冲区
				if(recvBuff->getRemain() > 0)
                {
                    iRetCode = this_->m_socket->Recv(recvBuff->getBuffer() + recvBuff->getPosition(),recvBuff->getRemain());
				}
                else //缓冲区满了~~
                {
                    this_->m_Status = SocketClient_DESTROY;
                    log("SocketClient::threadReceiveMessage：receive buff is full, receive thread stop!\n");
					this_->clearRecvQueue(); //清空已经接收到的数据
					this_->ReceiveStatus(MsgStatus_Error_Buffout);
					return;
                }
                
				if (iRetCode == -1) //结果没有读取到数据
				{
                    log("SocketClient::threadReceiveMessage:error recv read！\n");
					this_->m_Status = SocketClient_DESTROY; //线程停止
                    
					this_->ReceiveStatus(MsgStatus_Error_Read);
                    //this_->clearRecvQueue(); //清空已经接收到的数据
                    //this_->reConnect();      //线程重新运行
                    
					return;
				}
                else if(iRetCode == 0 && recvBuff->getRemain() > 0) //server 端关闭
                {
                    if(errno == 0) //服务器正常关闭连接
                    {
                        FD_ZERO(&fdRead);
                        //this_->m_socket->reConnect();
						this_->ReceiveStatus(MsgStatus_Error_ServerClose);
                        continue;
                    }
                    else
                    {
                        this_->m_Status = SocketClient_DESTROY;
                        log("SocketClient::threadReceiveMessage：server closed connection, receive thread stop!errorno:%d \n",errno);
                        this_->clearRecvQueue(); //清空已经接收到的数据
						this_->ReceiveStatus(MsgStatus_Error_ServerClose);
                        return;
                    }
				}
				else
				{
					gettimeofday(&lastReceiveDataTime, NULL);
					
					recvBuff->setPosition(recvBuff->getPosition()+ iRetCode);
					recvBuff->flip();
					int tmpOffset = 4;
					while(recvBuff->getRemain() > tmpOffset) // 正常接收到数据
                    {
						int pos = recvBuff->getPosition();
						int length= 0;
						length = recvBuff->getByte() << 8 | recvBuff->getByte();
						if(recvBuff->getRemain() +tmpOffset >= length) //已经全部收到
                        {
                            Message *msg = new Message;
							msg->status = 0;
							msg->data = new char[length+1];
							msg->sz = length;
							msg->data[length]='\0';
							recvBuff->get(msg->data, 0, length);
                            {
                                this_->m_sendqueue_mutex.lock();
                                this_->m_receivedMessageQueue.push(msg);
								this_->m_sendqueue_mutex.unlock();
                            }
						}
                        else if(length > recvBuff->getCapacity()) //包太大，缓冲区存放不小下
                        {
							this_->m_Status = SocketClient_DESTROY;
                            log("receive error message size(%d) greate capacity(%d), receive thread stop!\n",length,recvBuff->getCapacity());
							this_->clearRecvQueue(); //清空
							this_->ReceiveStatus(MsgStatus_Error_Buffout);
							return;
						}
                        else //没有接收完，继续接收
                        {
                            recvBuff->setPosition(pos);
							break;
						}
					}
					//
					recvBuff->compact();
				}
				
			}//end read
		}
        
	}
    m_bThreadRecvCreated = false;
}

void SocketClient::threadSendMessage()
{
    ByteBuffer* sendBuff = &m_cbSendBuf;
 	while (m_Status != SocketClient_DESTROY)
    {
 		if( m_Status == SocketClient_OK)
         {
             //发送缓冲器有遗留的数据要发送
             if(sendBuff->getPosition() > 0)
             {
                 sendBuff->flip(); //
                 int ret = m_socket->Send(sendBuff->getBuffer(), sendBuff->getLimit());
                 if(-1 == ret) //stop send and send error to server
                 {
                     m_Status = SocketClient_DESTROY;
                     log("send error");//发送数据，网络异常！
 					 ReceiveStatus(MsgStatus_Error_Send);
                     return;
                 }
                 sendBuff->setPosition(sendBuff->getPosition()+ret);
                 sendBuff->compact();
             }
             
             //循环发送消息队列的消息
             Message* msg = NULL;
             while(m_Status != SocketClient_DESTROY && m_sendMessageQueue.size()> 0)
             {
                 //读取消息
                 {
                     m_sendqueue_mutex.lock();
                     msg = m_sendMessageQueue.front();
                     m_sendMessageQueue.pop();
 					 m_sendqueue_mutex.unlock();
                 }
                 
                 if(msg->sz + sendBuff->getPosition() > sendBuff->getLimit())
                 {
                     m_Status = SocketClient_DESTROY;
                     log("send buffer is full, send thread stop!"); //发送太慢,或者发送包太大，网络环境有问题
 					 ReceiveStatus(MsgStatus_Error_SendBuff);
                     return;
                 }
                 //添加到缓冲区，并发送
                 //printf("send data:%s",msg->getContent());
 				 uint32_t tmp[2];
				 tmp[0] = (msg->sz >> 8) & 0xff;
				 tmp[1] = msg->sz & 0xff;
 				 sendBuff->putByte(tmp[0]);
				 sendBuff->putByte(tmp[1]);
                 sendBuff->put(msg->data,0,msg->sz);
                 sendBuff->flip();
                 int ret = m_socket->Send(sendBuff->getBuffer(),sendBuff->getLimit(),0);
                 if(ret == -1){
                     m_Status = SocketClient_DESTROY;
 					log("send error");//发送数据，网络异常！
 					ReceiveStatus(MsgStatus_Error_Send);
                     return;
                 }
                 sendBuff->setPosition(sendBuff->getPosition()+ret);
                 sendBuff->compact();
                 
 				delete msg->data;
                 delete msg;
             }
 		}
 		
         //等待5s 或者被唤醒 继续发送
         if(m_Status != SocketClient_DESTROY && m_sendMessageQueue.size() == 0)
         {
 			int second = 0;
             
             //如果缓冲区还有数据没有发送完，则 1s 后继续发送
             if(m_Status == SocketClient_OK && sendBuff->getPosition() > 0)
             {
 				second = 1;
             }
             else //或者等5s自动后苏醒
             {
 				second = 5;
             }
 			
 			if(m_Status != SocketClient_DESTROY && m_sendMessageQueue.size() == 0)
             {
 				std::unique_lock<std::mutex> lk(m_thread_cond_mutex); 
 				m_threadCond.wait_for(lk,std::chrono::seconds(second));
 			}
 		}
 		
 	}
	m_bThreadSendCreated = false;
}

void SocketClient::clearRecvQueue()
{
    m_recvqueue_mutex.lock();
    while( m_receivedMessageQueue.size()>0)
    {
        Message* msg = m_receivedMessageQueue.front();
        m_receivedMessageQueue.pop();
		delete msg->data;
        delete msg;
    }
	m_recvqueue_mutex.unlock();
}

void SocketClient::clearSendQueue()
{
    m_sendqueue_mutex.lock();
    while( m_sendMessageQueue.size()>0)
    {
        Message* msg = m_sendMessageQueue.front();
        m_sendMessageQueue.pop();
		delete msg->data;
        delete msg;
    }
	m_sendqueue_mutex.unlock();
}

void SocketClient::sendMessage(Message* msg)
{
	if(m_Status == SocketClient_DESTROY)
    {
        log("SocketClient::sendMessage  发送队列没有正常运行！");
		delete msg->data;
		delete msg;
		return;
	}
    
    {
        m_sendqueue_mutex.lock(); // 加发送队列锁
        m_sendMessageQueue.push(msg); //加入发送队列
		m_sendqueue_mutex.unlock();
    }
    
	if( m_Status == SocketClient_OK)
	{
		m_threadCond.notify_one();
	}
}

bool SocketClient::start()
{
	if( m_ip.length() < 1 || m_port == 0) return false;

	//创建套接字
	if(!m_socket->Create(AF_INET, SOCK_STREAM,0))
	{
		log("socket create error!");
		return false;
	}
	
	//连接
	if (!m_socket->Connect(m_ip.c_str(),m_port))
	{
		m_socket->Close();
		log("socket connect error,host:%s,ip:%d",m_ip.c_str(),m_port);
		return false;
	}
	ReceiveStatus(MsgStatus_Connect);
	//运行接收线程
	if( !m_bThreadRecvCreated )
	{
		pthread_t_receive = new std::thread(&SocketClient::threadReceiveMessage, this);
		pthread_t_receive->detach();
		m_bThreadRecvCreated = true;
	}

	m_Status = SocketClient_OK;
	
	if(!m_bThreadSendCreated)
    {
		pthread_t_send = new std::thread(&SocketClient::threadSendMessage, this);
		pthread_t_send->detach();
		m_bThreadSendCreated = true;
	}

	return true;
}

void SocketClient:: stop()
{
	m_Status = SocketClient_DESTROY;
	
 	m_threadCond.notify_one(); //发送信号，让其发送完成
	
	while (m_bThreadRecvCreated || m_bThreadSendCreated)
	{
		std::chrono::milliseconds dura( 10 );
		std::this_thread::sleep_for( dura );
	}
 	
 	//pthread_t_receive.join();//等待接收线程完成释放资源
	delete pthread_t_receive;
	
	//pthread_t_send->join();	 //等待发送线程完成释放资源
	delete pthread_t_send;
	m_bThreadSendCreated = false;
	
	//清空接收的消息
	while (!m_receivedMessageQueue.empty())
    {
		Message* m = m_receivedMessageQueue.front();
		m_receivedMessageQueue.pop();
		CC_SAFE_DELETE(m);
	}
	
    //清空发送队列
	while (!m_sendMessageQueue.empty())
    {
		Message* m = m_sendMessageQueue.front();
		m_sendMessageQueue.pop();
		CC_SAFE_DELETE(m);
	}
	
    m_socket->Close();//关闭套接字
	ReceiveStatus(MsgStatus_Close);
}

int SocketClient::bytesToInt( byte* bytes )
{
	int addr = bytes[3] & 0xFF;

	addr |= ((bytes[2] << 8) & 0xFF00);

	addr |= ((bytes[1] << 16) & 0xFF0000);

	addr |= ((bytes[0] << 24) & 0xFF000000);

	return addr;
}

byte* SocketClient::intToByte( int i )
{
	byte* abyte0 = new byte[4];

	abyte0[3] = (byte) (0xff & i);

	abyte0[2] = (byte) ((0xff00 & i) >> 8);

	abyte0[1] = (byte) ((0xff0000 & i) >> 16);

	abyte0[0] = (byte) ((0xff000000 & i) >> 24);

	return abyte0;
}

Message* SocketClient::popReceivedMessage()
{
	Message* msg = NULL;
	m_sendqueue_mutex.lock();
	if( m_receivedMessageQueue.size()>0){	
		msg = m_receivedMessageQueue.front();
		m_receivedMessageQueue.pop();
	}
	m_sendqueue_mutex.unlock();
	return msg;
}

void SocketClient::ReceiveStatus( MsgStatus s )
{
	Message *msg = new Message;
	msg->status = s;
	msg->data = NULL;
	msg->sz = 0;

	m_sendqueue_mutex.lock();
	m_receivedMessageQueue.push(msg);
	m_sendqueue_mutex.unlock();
}
