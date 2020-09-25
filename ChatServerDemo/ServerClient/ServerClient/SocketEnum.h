#pragma once
#ifndef __ENUMTYPE_H__
#define __ENUMTYPE_H__
struct SocketEnum
{
	typedef enum  //定义类型
	{
		Invalid,//无效的
		Tcp,
		Udp
	}SocketType;
	typedef enum
	{
		Error = -1,
		Success = 0,
		InvalidSocket,
		InvaliAddress,//无效的地址
		InvalidPort,//无效的端口
		ConnectionRefused,//连接被拒绝
		Timedout,//超过时限
		Ewouldblock,
		Notconnected,
		Einprogress,
		Interrupted,
		ConnectionAborted,//连接中止
		ProtocolError,//协议错误
		InvalidBuffer,//无效缓冲区
		ConnectionReset,
		AddressInUse,
		InvalidPointer,
		WSAStartupError,
		BindError,
		ListenError,
		UnknownError
	}SocketError;
	typedef enum
	{
		Receives = 0,
		Sends = 1,
		Both = 2,
	}ShutdownMode;
};
#endif __ENUMTYPE_H__