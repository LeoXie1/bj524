#pragma once
#ifndef __CSOCKET_H__
#define __CSOCKET_H__
#include <windows.h>
#include "SocketEnum.h"
#include <iostream>
using namespace std;
#include "ClientList.h"
#include <memory>
class ClientList;
class CMySocket
{
public:
	CMySocket(SocketEnum::SocketType _socketType = SocketEnum::Tcp);
	~CMySocket();
	bool Connect(const char* ip, int port);//����    port:�˿�
	int Send(char* pBuf, int len);//����
	int Receive(int strLen);//����
	bool SetBlocking(bool isBlockinng);//��������ģʽ
	bool ShutDown(SocketEnum::ShutdownMode mode);
	string GetData();//��ȡ��������
	SocketEnum::SocketError GetSocketError();
	void SetSocketHandle(SOCKET socket);//�����׽��־��
	void Close();//�ر�
	bool operator==(const CMySocket* socket);
	SOCKET GetSocket();
	bool IsExit();
	void diaoyong(ClientList* Clien2);
	ClientList* p;
private:
	void SetSocketError(SocketEnum::SocketError error);
	void SetSocketError(void);
	bool IsSocketValid(void);
	SOCKET csocket;
	bool isConnected;//����״̬
	struct sockaddr_in serverAddress;
	shared_ptr<string>m_pstrbuffer;
	 //���������
	int sendCount;//�������ݳ���
	int recvCount;//�������ݳ���
	bool isBlocking;//�Ƿ�������ģʽ
	SocketEnum::SocketError socketError;
	SocketEnum::SocketType socketType;
	WSADATA wsa;
	//ClientList* p;
};
#endif