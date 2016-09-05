#include "stdafx.h"
#include "Node.h"


CNode::CNode()
{
}


CNode::CNode(NODE_TYPE type, NODE_POSTION& postion) :
	_type(type),
	_postion(postion)
{

}

CNode::~CNode()
{
}

