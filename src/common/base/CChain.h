
#ifndef __CChain_H__
#define __CChain_H__

#include "CCommon.h"

namespace CBase
{
class Node {
public:
	void* data;
	class Node *next;

public:
	Node(): data(NULL), next(NULL) { };
	Node(void* ptr): data(ptr), next(NULL) { };
	~Node() {};
};

class CChainList {
public:
	Node* head;
private:
	int len;
public:
	CChainList() { head = NULL; len = 0; };
	~CChainList();

	int length();
	bool isEmpty();
	int addToHead(Node* node);
	int addToEnd(Node* node);
	bool cleanAll();
	bool delNode(Node *node);
	bool removeNode(Node *node);
	// index eg:0,1,2,...
	int getElem(int index, Node* &node, bool needRemove = false);
	int insertNode(int index, Node* node);
	Node * Reverse();
	//void dump();

	CChainList(const CChainList &);
	CChainList &operator=(const CChainList &);
};
}
#endif