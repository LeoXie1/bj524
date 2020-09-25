#pragma once
#ifndef __ENUMTYPE_H__
#define __ENUMTYPE_H__
struct SocketEnum
{
	typedef enum  //��������
	{
		Invalid,//��Ч��
		Tcp,
		Udp
	}SocketType;
	typedef enum
	{
		Error = -1,
		Success = 0,
		InvalidSocket,
		InvaliAddress,//��Ч�ĵ�ַ
		InvalidPort,//��Ч�Ķ˿�
		ConnectionRefused,//���ӱ��ܾ�
		Timedout,//����ʱ��
		Ewouldblock,
		Notconnected,
		Einprogress,
		Interrupted,
		ConnectionAborted,//������ֹ
		ProtocolError,//Э�����
		InvalidBuffer,//��Ч������
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