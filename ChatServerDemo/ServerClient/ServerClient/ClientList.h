#pragma once
#ifndef _CLIENTLIST_H_
#define _CLIENTLIST_H_
#include <vector>
#include "MySocket.h"
#include <assert.h>//assert.h�����ڷ���ʽ��̡����ԣ�Assertions����һ������ͨ����һ�����̣�routines������һ���꣨marcos����ÿ������ͨ����������������һ��������ʾʽ��a boolean expression����һ����Ϣ��a message����
class CMySocket;
class ClientList
{
public:
	typedef vector<CMySocket*>::iterator Iter;//typedef:���Ͷ���   iterator:������ 
	void Add(CMySocket* socket);
	int Count() const;
	CMySocket* operator[](size_t index);//index:ָ��
	void Remove(CMySocket* socket);//�Ƴ�
	Iter Find(CMySocket* socket);//����
	void Clear();
	void Broadcast(char* str, int len, CMySocket* s3);
	void BroadcastOffline(CMySocket* s3);
	void ShowVec(const vector<int>& valList);
	static ClientList* GetInstance()//����һ��ȫ�ֿͻ���list�б�  GetInstance()ʵ��
	{
		static ClientList instance;
		return &instance;//����ʵ����ַ
	}
	~ClientList();//����  �ͷ��ڴ�
private:
	static CRITICAL_SECTION g_cs;
	static vector<CMySocket*> _list;//list�б�����socket��̬ȫ�ֶ���
	ClientList();
	ClientList(const ClientList&);
	ClientList& operator=(const ClientList&);
};
#endif