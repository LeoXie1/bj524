// ServerClient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
// Server��ϰ��.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include <Windows.h>
#include <process.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
#include <vector>
#include "SServer.h"
#include "MySocket.h"
#include "ClientList.h"

void recv(PVOID pt)//����һ�������׽���ʵ���ĺ���
{
	CMySocket* csocket = (CMySocket*)pt;//����CMySocketָ�����ͣ��׽��֣�
	if (csocket != NULL)
	{
		int count = csocket->Receive(BUF_LEN);
		cout << "�����ֽڣ�" << count << endl;
		if (count == 0)
		{
			//����յ���ϢΪ�գ���ô��������
			char bufftmp[BUF_LEN] = { 0 };
			ClientList* list = ClientList::GetInstance();
			list->BroadcastOffline(csocket);
			list->Remove(csocket);
			cout << "һ���û����ߣ����ڵ���������Ϊ:" << list->Count() << endl;
			_endthread();//�û����ߣ���ֹ���������߳�
		}
	}
}

//�㲥�߳�
void sends(PVOID pt)
{
	ClientList* list = (ClientList*)pt;
	while (1)
	{
		char* buf = new char[BUF_LEN];
		cin >> buf;
		int bufSize = 0;
		while (buf[bufSize++] != '\0');
		for (int i = list->Count() - 1; i >= 0; i--)
		{
			(*list)[i]->Send(buf, bufSize);
		}
		delete buf;
	}
}
int main(int argc, char* argv[])
{
	SServer server;
	bool isStart = server.Start(1986);
	if (isStart)
	{
		cout << "server start success..." << endl;
	}
	else
	{
		cout << "server start error" << endl;
	}
	ClientList* list = ClientList::GetInstance();
	_beginthread(sends, 0, list);//����һ���̹߳㲥����
	while (1)
	{
		CMySocket* clientsocket = server.Accept();
		clientsocket->diaoyong(list);
		char welcome[512] = { 0 };
		sprintf_s(welcome, "��ӭClient(%d)", clientsocket->GetSocket());
		clientsocket->Send(welcome,strlen(welcome) + 1);
		list->Add(clientsocket);
		memset(welcome, 0, 512);
		sprintf_s(welcome, "��ӭClient��%d������������", clientsocket->GetSocket());
		cout << "������һ���û���������������Ϊ:" << list->Count() << endl;
		list->Broadcast(welcome, strlen(welcome)+1, clientsocket);
		_beginthread(recv, 0, clientsocket);//����һ���������ݵ��߳�
	}
	getchar();
	return 0;
}