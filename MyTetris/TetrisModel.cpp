#include "stdafx.h"
#include "TetrisModel.h"


CTetrisModel::CTetrisModel(void)
{
}


CTetrisModel::~CTetrisModel(void)
{
}

bool CTetrisModel::SetBackgound(int columns /*= 14*/, int rows /*= 22*/)
{
	bool ret = false;

	do 
	{
		SetColumns(columns);
		SetRows(rows);

		//��ʼ���������
		_nodeDatas.resize(GetRows());
		for (int i = 0; i < GetRows(); ++i)
		{
			_nodeDatas[i].resize(GetColumns());

			for (int j = 0; j < GetColumns(); ++j)
			{
				//��ʼ��Ϊ�հױ���
				CNode::NODE_POSTION postion;
				postion.row = i;
				postion.column = j;
				_nodeDatas[i][j] = new CNode(CNode::NODE_BLANK, postion);
				if (_nodeDatas[i][j] == NULL)
				{
					goto _END;
				}
			}
		}



		ret = true;
	} while (FALSE);

_END:

	return ret;
}

void CTetrisModel::SetWall(
	int leftWidth /*= 1*/,
	int rightWidth /*= 1*/, 
	int bottomWidth /*= 1*/, 
	int topWidth /*= 1*/)
{
	_leftWidth = leftWidth;
	_rightWidth = rightWidth;
	_bottomWidth = bottomWidth;
	_topWidth = topWidth;


	for (int iRow = 0; iRow < GetRows(); ++iRow)//��
	{
		for (int jColumn = 0; jColumn < GetColumns(); ++jColumn)//��
		{
			if (jColumn < _leftWidth)
			{
				_nodeDatas[iRow][jColumn]->SetType(CNode::NODE_WALL);
			}

			if (GetColumns() - jColumn < _rightWidth + 1)
			{
				_nodeDatas[iRow][jColumn]->SetType(CNode::NODE_WALL);
			}

			if (iRow < _topWidth)
			{
				_nodeDatas[iRow][jColumn]->SetType(CNode::NODE_WALL);
			}

			if (GetRows() - iRow < _bottomWidth + 1)
			{
				_nodeDatas[iRow][jColumn]->SetType(CNode::NODE_WALL);
			}

			if (
				jColumn < _leftWidth ||				    //��ǽ				
				(GetColumns() - jColumn - 1) < _rightWidth ||	//��ǽ
				iRow < _topWidth ||						//��ǽ
				(GetRows() - iRow - 1) < _bottomWidth			//��ǽ
				)
			{
				_nodeDatas[iRow][jColumn]->SetType(CNode::NODE_WALL);
			}
		}
	}
}

bool CTetrisModel::IsConflict(CNode::NODE_POSTION& topLeft, CNode::NODE_POSTION& bottomRight, NODE_MATRIX& tetris)
{
	return true;
}

CNode* CTetrisModel::GetData(CNode::NODE_POSTION postion)
{
	//λ�ò��ܳ���model��Χ
	if (IsValidPostion(postion))
	{
		return _nodeDatas[postion.row][postion.column];
	}

	return NULL;
}

bool CTetrisModel::IsValidPostion(CNode::NODE_POSTION postion)
{
	if (postion.row < GetRows() && postion.row >= 0 &&
		postion.column < GetColumns() && postion.column >= 0)
	{
		return true;
	}
	return false;
}
