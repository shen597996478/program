#include "CCommon.h"

#ifndef __CRBtree_H__
#define __CRBtree_H__


namespace CBase
{

enum RBColor
{
	RED,
	BLACK
};

struct BaseNode_t {
	// val must > 1, default -1
	int val;
	// data must be new or malloc
	void* data;
};

struct RBNode_t {
	RBColor color;
	struct RBNode_t *parent;
	struct RBNode_t *lchild;
	struct RBNode_t *rchild;
	BaseNode_t node;
};

class CRBtree
{

public:
	CRBtree();
	virtual ~CRBtree();
	bool put(struct BaseNode_t *node);
	struct BaseNode_t* getMin();
	struct BaseNode_t* getMax();
	struct BaseNode_t* get(int val);
	bool remove(int val);
	bool remove(struct BaseNode_t *node);
	bool has(int val);
	bool isEnpty();
	bool clear();
	void show();

private:
	void _show(struct RBNode_t *node);
	struct RBNode_t* _getMin(struct RBNode_t* _root);
	struct RBNode_t* _getMax(struct RBNode_t* _root);
	struct RBNode_t * find(int val);
	struct RBNode_t * leftSpin(struct RBNode_t *node); //return child node after spin.
	struct RBNode_t * rightSpin(struct RBNode_t *node);	//return child node after spin.
	struct RBNode_t * getFreeNode();
	void init(struct RBNode_t *node);
	bool setRBNode(struct RBNode_t *rbNode, struct BaseNode_t *node);
	int compareNode(struct RBNode_t *rbNodeA, struct RBNode_t *rbNodeB);
	void setColor(RBNode_t *rbNode, RBColor color);
	void replace_node(struct RBNode_t *src, struct RBNode_t *dst);
	struct RBNode_t* grandParent(struct RBNode_t *node);
	struct RBNode_t* uncle(struct RBNode_t *node);
	struct RBNode_t* sibling(struct RBNode_t *node);
	
	void insert_case1(struct RBNode_t *node);
	void insert_case2(struct RBNode_t *node);
	void insert_case3(struct RBNode_t *node);
	void insert_case4(struct RBNode_t *node);
	void insert_case5(struct RBNode_t *node);
	
	void delete_case1(struct RBNode_t *node);
	void delete_case2(struct RBNode_t *node);
	void delete_case3(struct RBNode_t *node);
	void delete_case4(struct RBNode_t *node);
	void delete_case5(struct RBNode_t *node);
	void delete_case6(struct RBNode_t *node);

private:
	struct RBNode_t *root;

	CRBtree(const CRBtree &);
	CRBtree &operator=(const CRBtree &);

};

}
#endif