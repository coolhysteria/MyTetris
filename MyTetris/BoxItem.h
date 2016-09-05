#pragma once
class CNode
{
public:
	CNode(void);
	~CNode(void);

	enum
	{
		NODE_WALL = 0x01,
		NODE_BOX,
		NODE_BLANK
	}NODE_TYPE;

	enum NODE_TYPE GetType() const;
	void SetType(enum NODE_TYPE type);

private:
	enum NODE_TYPE _type;
};

