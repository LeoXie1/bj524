#include "stdafx.h"
#include "SServer.h"
bool SServer::Start(int port)
{
	isStart = true;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)//��ʼ���׽���DLL       WSAStartup ��windows���첽�׽�����������������޹صı�̽ӿڣ�
	{
		SetSocketError(SocketEnum::WSAStartupError); //���ô�����Ϣ
		isStart = false;//������󷵻�false
	}
	if (isStart)//���ture
	{
		if ((ssocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)//INVALID_SOCKET �ǰ�socket���ó���Ч�׽���
		{
			SetSocketError(SocketEnum::InvalidSocket);//���ô�����Ϣ
			isStart = false;//������󷵻�false
		}
	}
	if (isStart)//���ture
	{
		//��ʼ��ָ�����ڴ�����
		memset(&serverAddress, 0, sizeof(sockaddr_in));//serverAddress �Ƿ����ڴ��ַ    sizeof(sockaddr_in) �Ƿ��ؿ�Ĵ�С
		serverAddress.sin_family = AF_INET;//��ʼ��
		serverAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//��ʼ��
		if (port > 0)
		{
			serverAddress.sin_port = htons(port);//��ʼ��
		}
		else
		{
			SetSocketError(SocketEnum::InvalidPort);//���ô�����Ϣ
			isStart = false;//����false
		}
	}
	if (isStart)//���ture
	{
		//��    //bind(���󶨵ĺ�������/����ָ��/��Ա����ָ��,������ֵ1,������ֵ2,...,������ֵn)
		if (bind(ssocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		{
			SetSocketError(SocketEnum::BindError);//���ô�����Ϣ
		}
		else
		{
			if (listen(ssocket, SOMAXCONN) == SOCKET_ERROR)//��������״̬    SOMAXCONN��һ��Ĭ��ֵ�������ֵ��
			{
				SetSocketError(SocketEnum::ListenError);//���ô�����Ϣ
			}
		}
	}
	return isStart; //����ture
}
void SServer::SetSocketError(SocketEnum::SocketError error)//����Ĵ������ Socket ��
{
	socketError = error;
}
CMySocket* SServer::Accept()//accept()����һ���׽ӿڽ��ܵ�һ�����ӣ������ؾ��
{
	CMySocket*csocket = new CMySocket();
	struct sockaddr_in clientAddress;//�����Ϳͻ���ͨ�ŵ��׽��ֵ�ַ
	int addrlen = sizeof(clientAddress);
	memset(&clientAddress, 0, addrlen);//��ʼ����ſͻ�����Ϣ���ڴ�
	SOCKET socket;
	if ((socket = accept(ssocket, (sockaddr*)&clientAddress, &addrlen)) != INVALID_SOCKET)
	{
		csocket->SetSocketHandle(socket);//���þ��
	}
	return csocket;//���ؾ��
}
SServer::~SServer()//����
{
	Close();
}
bool SServer::ShutDown(SocketEnum::ShutdownMode mode)//�ر�socket
{
	SocketEnum::SocketError nRetVal = (SocketEnum::SocketError)shutdown(ssocket, SocketEnum::Both);
	return (nRetVal == SocketEnum::Success) ? true : false;//�жϳɹ���񣬷���ture����false
}
void SServer::Close()//�ر�
{
	ShutDown(SocketEnum::Both);//����shutdown����ִ�йرղ���
	if (closesocket(ssocket) != SocketEnum::Error)
	{
		ssocket = INVALID_SOCKET;
	}
	WSACleanup();//�����׽���ռ�õ���Դ
}