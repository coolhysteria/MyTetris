#pragma once
#include <QObject>

class CNode : public QObject
{
	Q_OBJECT

public:
	enum NODE_TYPE
	{
		NODE_WALL = 0x01,
		NODE_STATIC_BOX,
		NODE_MOVEABLE_BOX,
		NODE_BLANK,
		NODE_BLANK_BOX
	};


	struct NODE_POSTION
	{
		int row;
		int column;
	};

	CNode();
	CNode(NODE_TYPE type, NODE_POSTION& postion);
	~CNode();


	CNode::NODE_TYPE GetType() const { return _type; }
	void SetType(CNode::NODE_TYPE val) { _type = val; }

	CNode::NODE_POSTION GetPostion() const { return _postion; }
	void SetPostion(CNode::NODE_POSTION val) { _postion = val; }

signals:
	void Update(CNode::NODE_TYPE type, CNode::NODE_POSTION postion);

private:
	NODE_TYPE _type;
	NODE_POSTION _postion;
};

