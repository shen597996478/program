#include "CRBtree.h"

namespace CBase
{

CRBtree::CRBtree() {
	root = NULL;
}

CRBtree::~CRBtree() {
	clear();	
}

bool CRBtree::put(struct BaseNode_t *node) {
	struct RBNode_t* newRBNode = getFreeNode();
	bool bRet = setRBNode(newRBNode, node);
	if(bRet == false) return false;

	if(root == NULL) {
		root = newRBNode;
		setColor(root, BLACK);
		return true;
	}

	struct RBNode_t* curRBNode = root;
	while(curRBNode != NULL) {
		int iCmpRet = compareNode(newRBNode, curRBNode);
		if(iCmpRet == GREATER) {
			if(curRBNode->rchild == NULL) {
				//insert new to rchind
				curRBNode->rchild = newRBNode;
				newRBNode->parent = curRBNode;
				break;
			}
			curRBNode = curRBNode->rchild;
		} else { // less than or equal
			if(curRBNode->lchild == NULL) {
				//insert new to lchind
				curRBNode->lchild = newRBNode;
				newRBNode->parent = curRBNode;
				break;
			}
			curRBNode = curRBNode->lchild;
		}
	}
	// after insert new node, modify rbTree.
	insert_case1(newRBNode);
	return true;

}

struct BaseNode_t* CRBtree::getMin() {
	struct RBNode_t *minNode = _getMin(root);

	if(minNode == NULL) {
		return NULL;
	} else {
		return &minNode->node;
	}
}

struct RBNode_t* CRBtree::_getMin(struct RBNode_t* _root) {
	struct RBNode_t *minNode = _root;
	while(true) {
		if(minNode == NULL || minNode->lchild == NULL) {
			break;
		}
		minNode = minNode->lchild;
	}
	if(minNode == NULL) {
		return NULL;
	} else {
		return minNode;
	}
}

struct BaseNode_t* CRBtree::getMax() {
	struct RBNode_t *maxNode = _getMax(root);
	if(maxNode == NULL) {
		return NULL;
	} else {
		return &maxNode->node;
	}
}

struct RBNode_t* CRBtree::_getMax(struct RBNode_t* _root) {
	struct RBNode_t *maxNode = _root;
	while(true) {
		if(maxNode == NULL || maxNode->rchild == NULL) {
			break;
		}
		maxNode = maxNode->rchild;
	}
	if(maxNode == NULL) {
		return NULL;
	} else {
		return maxNode;
	}
}

struct BaseNode_t* CRBtree::get(int val) {

	struct RBNode_t *curNode = find(val);

	if(curNode == NULL) {
		return NULL;
	} else {
		return &curNode->node;
	}

}

bool CRBtree::remove(int val) {
	struct RBNode_t *rmNode = find(val);
	
	if(rmNode == NULL) {
		return false;
	}

	struct RBNode_t *replaceRmNode = NULL, *child = NULL;
	if(rmNode->rchild != NULL) {
		replaceRmNode = _getMin(rmNode->rchild);
		child = replaceRmNode->lchild == NULL ? replaceRmNode->rchild : replaceRmNode->lchild;
	} else if (rmNode->lchild != NULL) {
		replaceRmNode = _getMax(rmNode->lchild);
		child = replaceRmNode->lchild == NULL ? replaceRmNode->rchild : replaceRmNode->lchild;

	} else {
		replaceRmNode = rmNode;
		child = replaceRmNode->lchild == NULL ? replaceRmNode->rchild : replaceRmNode->lchild;
	}

	struct RBNode_t *leafNode = NULL;

	if(child == NULL) {
		leafNode = getFreeNode();
		leafNode->parent = replaceRmNode;
		leafNode->color = BLACK;
		child = leafNode;
	}
	replace_node(replaceRmNode, child);
	if(rmNode != replaceRmNode) {
		// two children of rmNode is both leaf.
		replace_node(rmNode, replaceRmNode);
	}

	//case1: delete node is red, color of child is Black, needn't modify.
	//case2: delete node is black, color of child is red, only modify color of child.
	//case3: delete node is black, color of child is black, rbTree is broken, need complex modifications.
	if(replaceRmNode->color == BLACK) {
		if(child->color == RED) {
			child->color = BLACK;
		} else {
			delete_case1(child);
		}
	} else {
		// replaceRM node is red, only delete, if use leaf node, later delete leaf node.
	}

	replaceRmNode->color = rmNode->color;
	if(rmNode->parent == NULL) {
		root = replaceRmNode;
	}
	// if we use leaf node, we need change leaf to NULL
	if(child == leafNode) {
		if(child->parent->lchild == leafNode) {
			child->parent->lchild = NULL;
		} else {
			child->parent->rchild = NULL;
		}
		delete leafNode;
	}
	delete rmNode;

	return true;
}

bool CRBtree::remove(struct BaseNode_t *node) {
	remove(node->val);
	return true;
}

bool CRBtree::has(int val) {
	
	struct RBNode_t *curNode = find(val);

	if(curNode == NULL) {
		return false;
	} else {
		return true;
	}
}

bool CRBtree::isEnpty() {
	bool bEmpty = true;

	if(root != NULL) {
		bEmpty = false;
	}

	return bEmpty;
}

bool CRBtree::clear() {
	return true;
}

struct RBNode_t * CRBtree::find(int val) {
	struct RBNode_t *curNode = root;

	while(true) {
		if(curNode == NULL) { break;}

		if(curNode->node.val > val) {
			curNode = curNode->lchild;
		} else if (curNode->node.val < val) {
			curNode = curNode->rchild;
		} else { //curNode->node.val == val
			break;
		}
	}

	return curNode;
}
	
struct RBNode_t * CRBtree::leftSpin(struct RBNode_t *node) {
	struct RBNode_t *parent = node->parent;
	//struct RBNode_t *lchild = node->lchild;
	struct RBNode_t *rchild = node->rchild;
	if(rchild != NULL) {
		if(rchild->lchild != NULL) {
			node->rchild = rchild->lchild;
			rchild->lchild->parent = node;
		} else {
			node->rchild = NULL;
		}
		
		rchild->lchild = node;
		node->parent = rchild;
		
		if(parent != NULL) {
			if(parent->lchild == node) {
				parent->lchild = rchild;
				if(rchild != NULL) {
					rchild->parent = parent;
				}
			} else {
				parent->rchild = rchild;
				if(rchild != NULL) {
					rchild->parent = parent;
				}
			}
			
		} else {
			// root node, parent node is NULL
			root = rchild;
			if(rchild != NULL) {
				rchild->parent = NULL;
			}
		}
	} else {
		return NULL;
	}

	return node;
}

struct RBNode_t * CRBtree::rightSpin(struct RBNode_t *node) {
	struct RBNode_t *parent = node->parent;
	struct RBNode_t *lchild = node->lchild;
	//struct RBNode_t *rchild = node->rchild;

	if(lchild != NULL) {

		if(lchild->rchild != NULL) {
			node->lchild = lchild->rchild;
			lchild->rchild->parent = node;
		} else {
			node->lchild = NULL;
		}
		
		lchild->rchild = node;
		node->parent = lchild->rchild;
		if(parent != NULL) {
			parent->rchild = lchild;
			if(parent->lchild == node) {
				parent->lchild = lchild;
				if(lchild != NULL) {
					lchild->parent = parent;
				}
			} else {
				parent->rchild = lchild;
				if(lchild != NULL) {
					lchild->parent = parent;
				}
			}
		} else {
			// root node, parent node is NULL
			root = lchild;
			if(lchild != NULL) {
				lchild->parent = NULL;
			}
		}
	} else {
		return NULL;
	}

	return node;
}

struct RBNode_t * CRBtree::getFreeNode() {
	struct RBNode_t* freeNode = new RBNode_t;
	init(freeNode);
	return freeNode;
}

void CRBtree::init(struct RBNode_t *node) {
	node->color = RED;
	node->node.val = -1;
	node->node.data = NULL;
	node->parent = NULL;
	node->lchild = NULL;
	node->rchild = NULL;
}

bool CRBtree::setRBNode(struct RBNode_t * rbNode, struct BaseNode_t *node)
{
	if(node->val == -1) {
		return false;
	}
	rbNode->node.val = node->val;
	rbNode->node.data = node->data;

	return true;
}

int CRBtree::compareNode(struct RBNode_t *rbNodeA, struct RBNode_t *rbNodeB)
{
	if(rbNodeA == NULL || rbNodeB == NULL) {
		return INVALID_INT;
	}

	if(rbNodeA->node.val > rbNodeB->node.val) {
		return GREATER;
	} else if (rbNodeA->node.val == rbNodeB->node.val) {
		return EQUAL;
	} else {
		return LESS;
	}
}

void CRBtree::setColor(struct RBNode_t *rbNode, RBColor color)
{
	rbNode->color = color;
}

void CRBtree::replace_node(struct RBNode_t *src, struct RBNode_t *dst)
{
	if(src->parent != dst) {
		dst->parent = src->parent;
		if(src->parent != NULL && src->parent->lchild == src) {
			src->parent->lchild = dst;
		} else if(src->parent != NULL && src->parent->rchild == src) {
			src->parent->rchild = dst;
		}
	}

	if(src->lchild != dst) {
		dst->lchild = src->lchild;
		if(src->lchild != NULL) {
			src->lchild->parent = dst;
		}
	}

	if(src->rchild != dst) {
		dst->rchild = src->rchild;
		if(src->rchild != NULL) {
			src->rchild->parent = dst;
		}
	}

}

struct RBNode_t* CRBtree::grandParent(struct RBNode_t *node)
{
	if(node->parent != NULL) {
		return node->parent->parent;
	} else {
		return NULL;
	}
}

struct RBNode_t* CRBtree::uncle(struct RBNode_t *node)
{
	struct RBNode_t* gp = grandParent(node);
	if(gp == NULL) {
		return NULL;
	} else if (node->parent == gp->lchild) {
		return gp->rchild;
	} else {
		return gp->lchild;
	}
}

struct RBNode_t* CRBtree::sibling(struct RBNode_t *node)
{
	if(node->parent == NULL) {
		return NULL;
	} else if (node == node->parent->lchild) {
		return node->parent->rchild;
	} else {
		return node->parent->lchild;
	}
}

void CRBtree::insert_case1(struct RBNode_t *node)
{
	if(node->parent == NULL) {
		node->color = BLACK;
	} else {
		insert_case2(node);
	}
}

void CRBtree::insert_case2(struct RBNode_t *node)
{
	if(node->parent->color == BLACK) {
		return;
	} else {
		insert_case3(node);
	}
}

void CRBtree::insert_case3(struct RBNode_t *node)
{
	//parent is RED
	if(uncle(node) != NULL && uncle(node)->color == RED) {
		node->parent->color = BLACK;
		uncle(node)->color = BLACK;
		grandParent(node)->color = RED;
		insert_case1(grandParent(node));
	} else {
		insert_case4(node);
	}
}

void CRBtree::insert_case4(struct RBNode_t *node)
{
	if(node == node->parent->rchild && node->parent == grandParent(node)->lchild) {
		leftSpin(node->parent);
		// now, child node is previous parent node.
		node = node->lchild;
	} else if (node == node->parent->lchild && node->parent == grandParent(node)->rchild) {
		rightSpin(node->parent);
		// now, child node is previous parent node.
		node = node->rchild;
	}

	insert_case5(node);
}

void CRBtree::insert_case5(struct RBNode_t *node)
{
	grandParent(node)->color = RED;
	node->parent->color = BLACK;
	if(node == node->parent->lchild && node->parent == grandParent(node)->lchild) {
		rightSpin(grandParent(node));
	} else {
		leftSpin(grandParent(node));
	}
}

void CRBtree::delete_case1(struct RBNode_t *node)
{
	if(node->parent != NULL) {
		delete_case2(node);
	}
}

void CRBtree::delete_case2(struct RBNode_t *node)
{
	struct RBNode_t* s = sibling(node);

	if(s->color == RED) {
		s->color = BLACK;
		s->parent->color = RED;
		if(node == node->parent->lchild) {
			leftSpin(node->parent);
		} else {
			rightSpin(node->parent);
		}
	} else { // BLACK
		delete_case3(node);
	}
}

void CRBtree::delete_case3(struct RBNode_t *node)
{
	struct RBNode_t* s = sibling(node);
	if(s->parent->color == BLACK
		&& s->color == BLACK
		&& (s->lchild != NULL && s->lchild->color == BLACK)
		&& (s->rchild != NULL && s->rchild->color == BLACK)) {
		s->color = RED;
		delete_case1(node->parent);
	} else {
		delete_case4(node);
	}
}

void CRBtree::delete_case4(struct RBNode_t *node)
{
	struct RBNode_t* s = sibling(node);
	if(s->parent->color == RED
		&& s->color == BLACK
		&& (s->lchild != NULL && s->lchild->color == BLACK)
		&& (s->rchild != NULL && s->rchild->color == BLACK)) {
		s->color = RED;
		s->parent->color = BLACK;
	} else {
		delete_case5(node);
	}
}

void CRBtree::delete_case5(struct RBNode_t *node)
{
	struct RBNode_t* s = sibling(node);

	if(node == node->parent->lchild
		&& s->lchild->color == RED
		&& s->rchild->color == BLACK) {
		
		s->color = RED;
		s->lchild->color = BLACK;
		rightSpin(s);
	
	} else if (node == node->parent->rchild
		&& s->rchild->color == RED
		&& s->lchild->color == BLACK) {
		
		s->color = RED;
		s->rchild->color = BLACK;
		leftSpin(s);
	
	}

	delete_case6(node);

}

void CRBtree::delete_case6(struct RBNode_t *node)
{
	struct RBNode_t* s = sibling(node);
	s->color = node->parent->color;
	node->parent->color = BLACK;

	if(node == node->parent->lchild) {
		s->rchild->color = BLACK;
		leftSpin(node->parent);
	} else {
		s->lchild->color = BLACK;
		rightSpin(node->parent);
	}
}

void CRBtree::show()
{
	LOGD(LOG_TAG, "===================show()===================");
	_show(root);
}

void CRBtree::_show(struct RBNode_t *node)
{
	if(node != NULL) {
		LOGD(LOG_TAG, "p:%d, n:%d, lc:%d, rc:%d, color:%s", (node->parent?node->parent->node.val:-1) 
															, node->node.val
															, (node->lchild? node->lchild->node.val:-1)
															, (node->rchild?node->rchild->node.val:-1)
															, (node->color == RED?"RED":"BLACK"));
		_show(node->lchild);
		_show(node->rchild);

	}
}

}