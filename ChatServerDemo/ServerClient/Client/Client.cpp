// Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <process.h>
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib") 
using namespace std;
#define BUF_LEN 1024

bool g_clientExit = false;
int g_myID = 0;
void recvth(PVOID pt)//定义一个接收套接字实例的函数
{
	SOCKET* csocket = (SOCKET*)pt;//定义CMySocket指针类型（套接字）
	if (csocket != NULL)
	{
		do 
		{
			int recvCount = 0;
			char buffer[BUF_LEN] = { 0 };
			recvCount = recv(*csocket, buffer, BUF_LEN, 0);
			buffer[recvCount - 1] = '\0';
			if (strlen(buffer)>0)
			{
				////wushuju 
				if (errno == EAGAIN)
				{
					recvCount = 1;
				}
				else
				{
					//获取服务socket ID 
					if (g_myID ==0)
					{
						//欢迎Client(%d)
						string strtmp = buffer;
						int nindex = strtmp.find('(');
						char myID[12] = { 0 };
						strtmp =strtmp.substr(nindex + 1,strtmp.length()-nindex-2);
						sprintf_s(myID, "%s", strtmp.c_str());
						g_myID = atoi(myID);
					}
					cout << buffer << endl;
				}
			}

		} while (true);
		
	}
}

//发送线程
void sends(PVOID pt)
{
	SOCKET* csocket = (SOCKET*)pt;
	while (1)
	{
		char buf[BUF_LEN] = { 0 };
		sprintf_s(buf, "Client(%d):", g_myID);
		cout << buf;
		cin >> buf;
		string tmp = buf;
		if (tmp == "exit")
		{
			int ncount = send(*csocket, buf, BUF_LEN, 0);
			g_clientExit = true;
			delete[] buf;
			_endthread();
			return;
		}
		int ncount = send(*csocket, buf, BUF_LEN, 0);
		
	}
}

int main()
{
	bool bres = true;
	WSADATA wsa;
	int port = 1986;
	struct sockaddr_in serverAddress;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)//初始化套接字DLL
	{
		bres = false;//失败返回false
		return 0;
	}
	memset(&serverAddress, 0, sizeof(sockaddr_in));//memset:内存
	SOCKET csocket(INVALID_SOCKET);
	serverAddress.sin_family = AF_INET;
	long lip = inet_addr("127.0.0.1");
	if (lip == INADDR_NONE)
	{
		cout << "SocketEnum::InvaliAddress)" << endl;
		bres = false;
		return 0;
	}
	if (port < 0)//如果端口号小于0
	{
		cout << "SocketEnum::InvalidPort)"<<endl;
		return  0;
	}
	if ((csocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)//
	{
		cout << "SocketEnum::InvalidPort)" << endl;
		return  0;
	}

	serverAddress.sin_addr.S_un.S_addr = lip;
	serverAddress.sin_port = htons(port);
	if (connect(csocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		cout << "connect error"<<endl;
		//启动接受线程
	}
	else
	{
		cout << "connect sucess" << endl;
		_beginthread(recvth, 0, &csocket);
		//启动输入线程
		_beginthread(sends, 0, &csocket);
	}

	do 
	{
		Sleep(1000);
	} while (!g_clientExit);
	//关闭
	closesocket(csocket);
	//	7	终止
	WSACleanup();
	cout << "客户端退出" << endl;
	return 0;

}

