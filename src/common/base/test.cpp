#include "CCommon.h"
#include <string>
#include "unistd.h"
#include "CChain.h"
#include "CRBtree.h"
#include "CLog.h"
#include "CMessage.h"
#include "CHandler.h"

using namespace std;
using namespace CBase;
extern void show_command();
extern int process(int command);
extern void testChain();
extern void testRBtree();
extern void testCHandler();
int main(int argc, char **argv)
{
	int command;

	while(1) {
		cout << "Please input test command:" << endl;
		show_command();
		cin >> command;
		process(command);
	}
	return 0;
}

void show_command()
{
	string show_info = "Command list:\n";
	show_info = show_info 
				+ "1. Queue test;\n"
				+ "2. Chain test;\n"
				+ "3. RBTree test;\n"
				+ "4. Hnadler test;\n";

	cout << show_info << endl;
}

int process(int command)
{
	switch(command) {
		case 1: {
			cout << "start test Queue." << endl;
		}
		break;

		case 2: {
			cout << "start test Chain." << endl;
			testChain();
		}
		break;

		case 3: {
			cout << "start test RBtree." << endl;
			testRBtree();
		}
		break;

		case 4: {
			cout << "start test Hnadler." << endl;
			testCHandler();
		}
		break;

		case 5: {

		}
		break;

		case 6: {

		}
		break;

		case 7: {

		}
		break;

		case 8: {

		}
		break;

		case 9: {

		}
		break;

		case 10: {

		}
		break;

		case 11: {

		}
		break;

		default: {


		}
		break;
	}
	return 0;
}

void inttostr(const int &int_temp,std::string &string_temp)  
{  
    std::stringstream stream;
    stream<<int_temp;
    string_temp=stream.str();
} 

void testChain()
{
	CChainList CChainList;
	/*
	CChainList.addToEnd(new Node(1));
	CChainList.addToEnd(new Node(2));
	CChainList.addToEnd(new Node(3));
	CChainList.dump();
	cout << "===============================" << endl;

	CChainList.addToHead(new Node(11));
	CChainList.dump();
	cout << "===============================" << endl;

	Node tmpNode(2);
	CChainList.delNode(&tmpNode);
	CChainList.dump();
	cout << "===============================" << endl;

	CChainList.insertNode(2,new Node(4));
	CChainList.dump();
	cout << "===============================" << endl;

	Node* pTmpNode;
	CChainList.getElem(1, pTmpNode);
	string tmpStr;
	cout << "item1:" << pTmpNode->data << endl;
	CChainList.dump();
	cout << "===============================" << endl;

	CChainList.cleanAll();
	CChainList.dump();
	cout << "===============================" << endl;
	*/
}


void testRBtree()
{
	__AUTO_LOG__
	cout << "===============================start" << endl;
	CRBtree *prb = new CRBtree();
	CRBtree rbtree;
	struct BaseNode_t data1 = {50,NULL};
	struct BaseNode_t data2 = {35,NULL};
	struct BaseNode_t data3 = {78,NULL};
	struct BaseNode_t data4 = {27,NULL};
	struct BaseNode_t data5 = {45,NULL};
	struct BaseNode_t data6 = {56,NULL};
	struct BaseNode_t data7 = {90,NULL};
	struct BaseNode_t data8 = {40,NULL};
	struct BaseNode_t data9 = {48,NULL};
	struct BaseNode_t data10 = {10,NULL};

	rbtree.put(&data1);
	rbtree.put(&data2);
	rbtree.put(&data3);
	rbtree.put(&data4);
	rbtree.put(&data5);
	rbtree.put(&data6);
	rbtree.put(&data7);
	rbtree.put(&data8);
	rbtree.put(&data9);
	rbtree.put(&data10);
	rbtree.show();
	rbtree.remove(10);
	rbtree.show();
	rbtree.remove(27);
	rbtree.show();
	rbtree.remove(50);
	rbtree.show();
	rbtree.remove(78);
	rbtree.show();
	rbtree.remove(90);
	rbtree.show();
	rbtree.remove(35);
	rbtree.show();
	cout << "===============================end" << endl;
}

void testCHandler()
{
	CHandler mhandle("testHandler");
	mhandle.start();
	sleep(1);
	CMessage* msg = mhandle.getMessage();
	mhandle.sendMessage(msg);
	//sleep(1);
	mhandle.sendMessage(mhandle.getMessage());
	//sleep(1);
	mhandle.sendMessage(mhandle.getMessage());
}