#include "stdafx.h"
#include "SServer.h"
bool SServer::Start(int port)
{
	isStart = true;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)//初始化套接字DLL       WSAStartup 是windows的异步套接字启动命令（与网络无关的编程接口）
	{
		SetSocketError(SocketEnum::WSAStartupError); //设置错误信息
		isStart = false;//如果错误返回false
	}
	if (isStart)//如果ture
	{
		if ((ssocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)//INVALID_SOCKET 是把socket设置成无效套接字
		{
			SetSocketError(SocketEnum::InvalidSocket);//设置错误信息
			isStart = false;//如果错误返回false
		}
	}
	if (isStart)//如果ture
	{
		//初始化指定的内存区域
		memset(&serverAddress, 0, sizeof(sockaddr_in));//serverAddress 是返回内存地址    sizeof(sockaddr_in) 是返回块的大小
		serverAddress.sin_family = AF_INET;//初始化
		serverAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//初始化
		if (port > 0)
		{
			serverAddress.sin_port = htons(port);//初始化
		}
		else
		{
			SetSocketError(SocketEnum::InvalidPort);//设置错误信息
			isStart = false;//返回false
		}
	}
	if (isStart)//如果ture
	{
		//绑定    //bind(待绑定的函数对象/函数指针/成员函数指针,参数绑定值1,参数绑定值2,...,参数绑定值n)
		if (bind(ssocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		{
			SetSocketError(SocketEnum::BindError);//设置错误信息
		}
		else
		{
			if (listen(ssocket, SOMAXCONN) == SOCKET_ERROR)//进入侦听状态    SOMAXCONN是一个默认值（有最大值）
			{
				SetSocketError(SocketEnum::ListenError);//设置错误信息
			}
		}
	}
	return isStart; //返回ture
}
void SServer::SetSocketError(SocketEnum::SocketError error)//定义的错误代码 Socket 类
{
	socketError = error;
}
CMySocket* SServer::Accept()//accept()是在一个套接口接受的一个连接，并返回句柄
{
	CMySocket*csocket = new CMySocket();
	struct sockaddr_in clientAddress;//用来和客户端通信的套接字地址
	int addrlen = sizeof(clientAddress);
	memset(&clientAddress, 0, addrlen);//初始化存放客户端信息的内存
	SOCKET socket;
	if ((socket = accept(ssocket, (sockaddr*)&clientAddress, &addrlen)) != INVALID_SOCKET)
	{
		csocket->SetSocketHandle(socket);//设置句柄
	}
	return csocket;//返回句柄
}
SServer::~SServer()//析构
{
	Close();
}
bool SServer::ShutDown(SocketEnum::ShutdownMode mode)//关闭socket
{
	SocketEnum::SocketError nRetVal = (SocketEnum::SocketError)shutdown(ssocket, SocketEnum::Both);
	return (nRetVal == SocketEnum::Success) ? true : false;//判断成功与否，返回ture或者false
}
void SServer::Close()//关闭
{
	ShutDown(SocketEnum::Both);//调用shutdown函数执行关闭操作
	if (closesocket(ssocket) != SocketEnum::Error)
	{
		ssocket = INVALID_SOCKET;
	}
	WSACleanup();//清理套接字占用的资源
}