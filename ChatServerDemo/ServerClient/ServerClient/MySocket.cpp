#include "stdafx.h"
#include "MySocket.h"
CMySocket::CMySocket(SocketEnum::SocketType _socketType) :csocket(INVALID_SOCKET), isConnected(false), m_pstrbuffer(NULL), sendCount(0), recvCount(0), isBlocking(true), socketError(SocketEnum::InvalidSocket), socketType(_socketType) {}
bool CMySocket::Connect(const char* ip, int port)
{
	isConnected = true;
	socketError = SocketEnum::Success;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)//��ʼ���׽���DLL
	{
		SetSocketError(SocketEnum::WSAStartupError);//����
		isConnected = false;//ʧ�ܷ���false
	}
	if (isConnected)//���true
	{
		if ((csocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)//
		{
			SetSocketError();
			isConnected = false;//����ʧ��false
		}
	}
	if (isConnected)//���true
	{
		memset(&serverAddress, 0, sizeof(sockaddr_in));//memset:�ڴ�
		serverAddress.sin_family = AF_INET;
		long lip = inet_addr(ip);
		if (lip == INADDR_NONE)
		{
			SetSocketError(SocketEnum::InvaliAddress);
			isConnected = false;
		}
		else
		{
			if (port < 0)//����˿ں�С��0
			{
				SetSocketError(SocketEnum::InvalidPort);
				isConnected = false;//����ʧ��false
			}
			else
			{
				serverAddress.sin_addr.S_un.S_addr = lip;
				serverAddress.sin_port = htons(port);
				if (connect(csocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
				{
					SetSocketError();
					isConnected = false;//����ʧ��false
				}
			}
		}
	}
	return isConnected;//���سɹ�true
}
//��������ģʽ
bool CMySocket::SetBlocking(bool isBlock)
{
	int block = isBlock ? 0 : 1;
	if (ioctlsocket(csocket, FIONBIO, (ULONG*)&block) != 0)
	{
		return false;
	}
	isBlocking = isBlock;
	return true;
}
int CMySocket::Send(char* pBuf, int len)
{
	if (!IsSocketValid() || !isConnected)
	{
		return 0;
	}
	if (pBuf == NULL || len < 1)
	{
		return 0;
	}
	sendCount = send(csocket, pBuf, len, 0);
	if (sendCount <= 0)
	{
		cout << GetSocketError() << endl;
	}
	return sendCount;
}
//Receive:�յ�������;
int CMySocket::Receive(int strLen)//�����׽���
{
	recvCount = 0;
	if (!IsSocketValid() || !isConnected)
	{
		return recvCount;
	}
	if (strLen < 1)
	{
		return recvCount;
	}
	
	SetSocketError(SocketEnum::Success);
	while (1)
	{
		char buffer[BUF_LEN] = { 0 };
		recvCount = recv(csocket, buffer, BUF_LEN, 0);
		if (recvCount <0)
		{
			return 0;
		}
		buffer[recvCount - 1] = '\0';
		m_pstrbuffer = make_shared<string>(buffer);
		if (m_pstrbuffer->length() > 0)
		{
			////wushuju 
			if (errno == EAGAIN)
			{
				recvCount = 1;
			}
			else
			{
				if (IsExit())
				{

					Send(buffer, recvCount);
					recvCount = 0;
					break;
				}
				else
				{
					char buftemp[BUF_LEN] = { 0 };
					sprintf_s(buftemp, "Client(%d):%s", this->GetSocket(), buffer);
					p->Broadcast(buftemp, strlen(buftemp)+1, this);
					cout << buftemp << endl;
				}
			}
			
			
		}
	}
	
	return recvCount;
}
void CMySocket::diaoyong(ClientList* Clien2)
{
	p = Clien2;
}

SOCKET CMySocket::GetSocket()
{
	return csocket;
}

bool CMySocket::IsExit()
{
	int len = m_pstrbuffer->length();
	int i = 0;
	int size = 4;
	return (*m_pstrbuffer == "exit");
}
//���ô�����Ϣ
void CMySocket::SetSocketError(SocketEnum::SocketError error)
{
	socketError = error;
}
void CMySocket::SetSocketError(void)//ö��socket���صĴ�����Ϣ
{
	int nError = WSAGetLastError();
	switch (nError)
	{
	case EXIT_SUCCESS:
		SetSocketError(SocketEnum::Success);
		break;
	case WSAEBADF:
	case WSAENOTCONN:
		SetSocketError(SocketEnum::Notconnected);
		break;
	case WSAEINTR:
		SetSocketError(SocketEnum::Interrupted);
		break;
	case WSAEACCES:
	case WSAEAFNOSUPPORT:
	case WSAEINVAL:
	case WSAEMFILE:
	case WSAENOBUFS:
	case WSAEPROTONOSUPPORT:
		SetSocketError(SocketEnum::InvalidSocket);
		break;
	case WSAECONNREFUSED:
		SetSocketError(SocketEnum::ConnectionRefused);
		break;
	case WSAETIMEDOUT:
		SetSocketError(SocketEnum::Timedout);
		break;
	case WSAEINPROGRESS:
		SetSocketError(SocketEnum::Einprogress);
		break;
	case WSAECONNABORTED:
		SetSocketError(SocketEnum::ConnectionAborted);
		break;
	case WSAEWOULDBLOCK:
		SetSocketError(SocketEnum::Ewouldblock);
		break;
	case WSAENOTSOCK:
		SetSocketError(SocketEnum::InvalidSocket);
		break;
	case WSAECONNRESET:
		SetSocketError(SocketEnum::ConnectionReset);
		break;
	case WSANO_DATA:
		SetSocketError(SocketEnum::InvaliAddress);
		break;
	case WSAEADDRINUSE:
		SetSocketError(SocketEnum::AddressInUse);
		break;
	case WSAEFAULT:
		SetSocketError(SocketEnum::InvalidPointer);
		break;
	default:
		SetSocketError(SocketEnum::UnknownError);
		break;
	}
}
//valid:��Ч��
bool CMySocket::IsSocketValid(void)//��Ч���׽���
{
	return socketError == SocketEnum::Success;//����Success��Ϣ
}
SocketEnum::SocketError CMySocket::GetSocketError()
{
	return socketError;
}
CMySocket::~CMySocket()
{
	Close();//�ر�
}
void CMySocket::Close()
{
	
	ShutDown(SocketEnum::Both);
	if (closesocket(csocket) != SocketEnum::Error)
	{
		csocket = INVALID_SOCKET;
	}
	/* WSACleanup();//�����׽���ռ�õ���Դ*/
}
//ShutDown:�ػ�      mode:��ʽ
bool CMySocket::ShutDown(SocketEnum::ShutdownMode mode)
{
	SocketEnum::SocketError nRetVal = (SocketEnum::SocketError)shutdown(csocket, SocketEnum::Both);
	SetSocketError();
	return(nRetVal == SocketEnum::Success) ? true : false;
}
string CMySocket::GetData()
{

	return (*m_pstrbuffer);
}
void CMySocket::SetSocketHandle(SOCKET socket)
{
	if (socket != SOCKET_ERROR)
	{
		csocket = socket;
		isConnected = true;
		socketError = SocketEnum::Success;
	}
}
bool CMySocket::operator==(const CMySocket* socket)
{
	return csocket == socket->csocket;
}
