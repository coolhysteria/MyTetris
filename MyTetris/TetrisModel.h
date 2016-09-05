#pragma once

#include <QObject>
#include <vector>
#include "Node.h"

using namespace std;

class CTetrisModel : public QObject
{
	Q_OBJECT

public:

	typedef vector<vector<CNode*>> NODE_MATRIX;
	CTetrisModel(void);
	~CTetrisModel(void);

	int GetColumns() const { return _columns; }
	void SetColumns(int val) { _columns = val; }
	int GetRows() const { return _rows; }
	void SetRows(int val) { _rows = val; }

	bool SetBackgound(int columns = 14, int rows = 22);
	void SetWall(int leftWidth = 1, int rightWidth = 1, int bottomWidth = 1, int topWidth = 1);

	bool IsConflict(CNode::NODE_POSTION& topLeft, CNode::NODE_POSTION& bottomRight, NODE_MATRIX& tetris);

	CNode* GetData(CNode::NODE_POSTION postion);

	bool IsValidPostion(CNode::NODE_POSTION postion);

	int GetLeftWidth() const { return _leftWidth;  }
	int GetRightWidth() const { return _rightWidth; }

private:
	int _columns;
	int _rows;
	int _leftWidth;
	int _rightWidth;
	int _bottomWidth;
	int _topWidth;

	NODE_MATRIX _nodeDatas;
};