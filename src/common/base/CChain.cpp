#include "CChain.h"

namespace CBase
{
CChainList::~CChainList()
{

}

bool CChainList::isEmpty()
{
	bool result = false;
	if(len <= 0 || head == NULL) {
		result = true;
	}

	return result;
}

int CChainList::length()
{
	return len;
}

int CChainList::addToHead(Node* node)
{
	node->next = head;
	head = node;
	++ len;
	return 0;
}

int CChainList::addToEnd(Node* node)
{
	Node* endNode = head;
	

	if(head == NULL) {
		head = node;
	} else {
		while(endNode->next != NULL) {
			endNode = endNode->next;
		}
		endNode->next = node;
		node->next = NULL;
	}
	++ len;
	return 0;
}

bool CChainList::cleanAll()
{
	Node* tmpNode;
	while(head != NULL) {
		tmpNode = head;
		head = head->next;
		len --;
		delete tmpNode;
	}

	return true;
}

bool CChainList::delNode(Node *node)
{
	bool result = false;
	
	Node* tmpNode = head, *preNode = NULL;
	while(tmpNode != NULL) {
		if(tmpNode->data != node->data) {
			preNode = tmpNode;
			tmpNode = tmpNode->next;
			continue;
		} else {
			if(preNode != NULL) {
				// the node is not head node.
				preNode->next = tmpNode->next;
			} else {
				head = tmpNode->next;
			}
			delete tmpNode;
			--len;
			result = true;
			break;
		}
	}

	return result;
}

bool CChainList::removeNode(Node *node)
{
	bool result = false;
	
	Node* tmpNode = head, *preNode = NULL;
	if(tmpNode) {
		LOGW(LOG_TAG, "node:[%p] tmpNode:[%p] next:[%p] next->next:[%p]", node, tmpNode, tmpNode->next, tmpNode->next->next);
	}
	while(tmpNode != NULL) {

		if(tmpNode != node) {
			preNode = tmpNode;
			tmpNode = tmpNode->next;
			continue;
		} else {
			if(preNode != NULL) {
				// the node is not head node.
				preNode->next = tmpNode->next;
			} else {
				head = tmpNode->next;
				LOGW(LOG_TAG, "head:[%p] next:[%p] next->next:[%p]", head, head->next, head->next->next);
			}
			--len;
			result = true;
			break;
		}
	}

	return result;
}

int CChainList::getElem(int index, Node* &node, bool needRemove)
{
	int result = -1;
	Node* curNode = head;
	Node* preNode = NULL;
	node = NULL;

	if(len <= index || index < 0) {
		return result;
	}
	for(int i = 0; i < index; ++ i) {
		if(curNode == NULL) {
			break;
		}
		preNode = curNode;
		curNode = curNode->next;
	}

	if(curNode != NULL) {
		result = 0;
		node = curNode;
		if(needRemove) {

			if(preNode != NULL) {
				// the node is not head node.
				preNode->next = curNode->next;
			} else {
				head = curNode->next;
			}
			-- len;
			// curNode->next = NULL;
		}
		//LOGW(LOG_TAG, "node:[%p] next:[%p]", node, node->next);
	} else {
		LOGW(LOG_TAG, "CChainList is Null!(head:[%p])", head);
	}

	return result;
}

int CChainList::insertNode(int index, Node* node)
{
	int result = -1;
	Node* curNode = head, *preNode = NULL;
	if(len < index || index < 0) {
		return result;
	}
	for(int i = 0; i < index; ++ i) {
		if(curNode == NULL) {
			break;
		}
		preNode = curNode;
		curNode = curNode->next;
	}

	if(preNode == NULL) {
		addToHead(node);
	} else {
		preNode->next = node;
		node->next = curNode;
	}
	result = 0;
	++len;

	return result;
}

Node * CChainList::Reverse()
{
	return NULL;
}



void int2str(const int &int_temp,std::string &string_temp)  
{  
    std::stringstream stream;  
    stream<<int_temp;  
    string_temp=stream.str();   //此处也可以用 stream>>string_temp  
}  


/*
void CChainList::dump()
{
	std::string str = "", tmpStr;
	Node* curNode = head;
	std::cout << "print chain list:" << std::endl;
	std::cout << "chain list length:" << length() <<std::endl;
	for(int i = 0; curNode != NULL; ++i) {
		int2str(i, tmpStr);
		str += tmpStr;
		str += " : ";
		int2str((int)curNode->data, tmpStr);
		str += tmpStr;
		str += "\n";
		curNode = curNode->next;
	}
	std::cout << str << std::endl;
}
*/
}