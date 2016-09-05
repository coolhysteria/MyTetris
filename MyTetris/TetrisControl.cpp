#include "stdafx.h"
#include "TetrisControl.h"

static char arrBox[][4] = 
{
	1,1,0,0,
	1,1,0,0,
	0,0,0,0,
	0,0,0,0,

	0,0,0,0,
	1,1,1,1,
	0,0,0,0,
	0,0,0,0,

	0,1,0,0,
	1,1,1,0,
	0,0,0,0,
	0,0,0,0,

	1,1,0,0,
	1,0,0,0,
	1,0,0,0,
	0,0,0,0,

	1,0,0,0,
	1,1,0,0,
	0,1,0,0,
	0,0,0,0,


	0,1,0,0,
	1,1,0,0,
	1,0,0,0,
	0,0,0,0,

	1,1,0,0,
	0,1,0,0,
	0,1,0,0,
	0,0,0,0
};


CTetrisControl::CTetrisControl(void)
{

}


CTetrisControl::~CTetrisControl(void)
{
}

bool CTetrisControl::Start()
{
	InitAllBoxSharp();


	_lineStaticBoxCounts.clear();
	for (int i = 0; i < _model->GetRows(); ++i)
	{
		_lineStaticBoxCounts.push_back(0);
	}

	CreateNewBox();


	_isStarted = true;

	return true;
}

bool CTetrisControl::IsStart() const
{
	return _isStarted;
}

void CTetrisControl::Stop()
{
	_isStarted = false;
}

void CTetrisControl::Restart()
{
	_isStarted = true;
}

void CTetrisControl::SetModel(CTetrisModel *model)
{
	_model = model;
}

bool CTetrisControl::MoveLeft()
{
	if (_isStarted == false)
	{
		return true;
	}

	CNode::NODE_POSTION nextPostion;
 	nextPostion = _currentBoxPostion;
	nextPostion.column -= 1;


	//�ж��¸�λ���Ƿ�����ƶ�
	if (CanMove(nextPostion, _currentBoxMatrix) == false)
	{
		return false;
	}

	//������ǰ����
	EreaseCurrentBoxRegion(_currentBoxPostion);

	//����������
	UpdateCurrentBoxNextRegion(nextPostion, _currentBoxMatrix);

	//���µ�ǰλ��
	_currentBoxPostion = nextPostion;

	return true;
}

bool CTetrisControl::MoveRight()
{
	if (_isStarted == false)
	{
		return true;
	}

	CNode::NODE_POSTION nextPostion;
	nextPostion = _currentBoxPostion;
	nextPostion.column += 1;


	//�ж��¸�λ���Ƿ�����ƶ�
	if (CanMove(nextPostion, _currentBoxMatrix) == false)
	{
		return false;
	}

	//������ǰ����
	EreaseCurrentBoxRegion(_currentBoxPostion);

	//����������
	UpdateCurrentBoxNextRegion(nextPostion, _currentBoxMatrix);

	//���µ�ǰλ��
	_currentBoxPostion = nextPostion;

	return true;
}

bool CTetrisControl::MoveDown()
{
	if (_isStarted == false)
	{
		return true;
	}

	CNode::NODE_POSTION nextPostion;
	nextPostion = _currentBoxPostion;
	nextPostion.row += 1;


	//�ж��¸�λ���Ƿ�����ƶ�
	if (CanMove(nextPostion, _currentBoxMatrix) == false)
	{
		//�̶�����
		FixBox(_currentBoxPostion, _currentBoxMatrix);

		//������
		ReleaseLines(_currentBoxPostion, _currentBoxMatrix);

		//�����·���
		CreateNewBox();

		return false;
	}

	//������ǰ����
	EreaseCurrentBoxRegion(_currentBoxPostion);

	//����������
	UpdateCurrentBoxNextRegion(nextPostion, _currentBoxMatrix);

	//���µ�ǰλ��
	_currentBoxPostion = nextPostion;

	return true;
}

bool CTetrisControl::Rotate(ROTATE_DIRECTION direction)
{
	if (_isStarted == false)
	{
		return true;
	}

	//���Ƶ�ǰ����
	CTetrisModel::NODE_MATRIX copidMatrix;
	CreateTempMatrix(copidMatrix);
	CopyMatrix(_currentBoxMatrix, copidMatrix);

	//��ת����
	RotateMatrix(copidMatrix, _currentBoxMatrix, direction);


	//�ж��¾����Ƿ�����ƶ�
	if (CanMove(_currentBoxPostion, copidMatrix) == false)
	{
		return false;
	}


	//���µ�ǰ����
	CopyMatrix(copidMatrix, _currentBoxMatrix);

	//������ǰ����
	EreaseCurrentBoxRegion(_currentBoxPostion);

	//����������
	UpdateCurrentBoxNextRegion(_currentBoxPostion, _currentBoxMatrix);

	return true;
}

bool CTetrisControl::MoveDownFast()
{
	return true;
}

bool CTetrisControl::CanMove(CNode::NODE_POSTION& nextPostion, CTetrisModel::NODE_MATRIX& currentBox) const
{
	//boxΪNODE_MOVEABLE_BOX��ʱ�򣬱���ΪNODE_BLANK
	CNode::NODE_POSTION posCur = nextPostion;
	for (int i = 0; i < currentBox.size(); ++i)
	{
		for (int j = 0; j < currentBox.size(); ++j)
		{
			if (_model->IsValidPostion(posCur))
			{
				if (currentBox[i][j]->GetType() == CNode::NODE_MOVEABLE_BOX)
				{
					if (
						_model->GetData(posCur)->GetType() == CNode::NODE_STATIC_BOX ||
						_model->GetData(posCur)->GetType() == CNode::NODE_WALL
						)
					{
						return false;
					}

				}
			}

			++posCur.column;
		}

		posCur.column = nextPostion.column;
		++posCur.row;
	}

	return true;
}

void CTetrisControl::FixBox(CNode::NODE_POSTION& postion, CTetrisModel::NODE_MATRIX& currentBox)
{
	CNode::NODE_POSTION posCur = postion;

	//��NODE_MOVEABLE_BOX�޸�ΪNODE_STATIC_BOX
	for (int i = 0; i < currentBox.size(); ++i)
	{
		for (int j = 0; j < currentBox.size(); ++j)
		{
			if (_model->IsValidPostion(posCur))
			{
				//NODE_MOVEABLE_BOX�ĳ�NODE_STATIC_BOX
				if (currentBox[i][j]->GetType() == CNode::NODE_MOVEABLE_BOX)
				{
					_model->GetData(posCur)->SetType(CNode::NODE_STATIC_BOX);

					//���ӵ�ǰ�е�NODE_STATIC_BOX����
					_lineStaticBoxCounts[posCur.row] += 1;

				}
				//NODE_BLANK_BOX����NODE_WALL NODE_STATIC_BOX�ĳ�NODE_BLANK
				else if (currentBox[i][j]->GetType() == CNode::NODE_BLANK_BOX)
				{
					if (_model->GetData(posCur)->GetType() != CNode::NODE_WALL && 
						_model->GetData(posCur)->GetType() != CNode::NODE_STATIC_BOX)
					{
						_model->GetData(posCur)->SetType(CNode::NODE_BLANK);
					}

				}
			}

			++posCur.column;
		}

		posCur.column = postion.column;
		++posCur.row;
	}
}


int CTetrisControl::ReleaseLines(CNode::NODE_POSTION& postion, CTetrisModel::NODE_MATRIX& currentBox)
{
	//��һ��ռ�����˵�����
	int totalFullLines = 0;
	int releaseCounts = _model->GetColumns() - _model->GetLeftWidth() - _model->GetRightWidth();
	int maxRowCounts = _model->GetRows();
	for (int iRow = postion.row; iRow < maxRowCounts; ++iRow)
	{
		//����ﵽ��������
		if (_lineStaticBoxCounts[iRow] == releaseCounts)
		{
			++totalFullLines;
			ReleaseLine(iRow);
		}
	}

	return totalFullLines;
}

bool CTetrisControl::ReleaseLine(int row)
{
	int iRow = row;
	//���ϱ���ֱ����ǰ��û��NODE_STATIC_BOX
	while (_lineStaticBoxCounts[iRow] > 0)
	{
		for (int iColumn = 0; iColumn < _model->GetColumns(); ++iColumn)
		{	
			//���㵱ǰ�е�λ��
			CNode::NODE_POSTION currentPos;
			currentPos.column = iColumn;
			currentPos.row = iRow;

			//��ȡ��һ�е�����
			CNode::NODE_POSTION beforeRowPos = currentPos;
			--beforeRowPos.row;
			CNode::NODE_TYPE typeBeforeRow = _model->GetData(beforeRowPos)->GetType();

			//����ǰ������Ϊ��һ��
			_model->GetData(currentPos)->SetType(typeBeforeRow);

		}

		_lineStaticBoxCounts[iRow] = _lineStaticBoxCounts[iRow - 1];

		--iRow;
	}


	return true;
}

bool CTetrisControl::CreateNewBox()
{
	int rows = _model->GetRows();
	int columns = _model->GetColumns();


	//�ڶ��У��ڶ���
	_currentBoxPostion.row = 2;
	_currentBoxPostion.column = columns / 2 -1;

	QTime time;
	time = QTime::currentTime();
	qsrand(time.msec() + time.second() * 1000);
	int randBoxIndex = qrand() % 6;

	_currentBoxMatrix = _tetrisAllSharp[randBoxIndex];

	CNode::NODE_POSTION postion = _currentBoxPostion;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (_currentBoxMatrix[i][j]->GetType() == CNode::NODE_MOVEABLE_BOX)
			{
				CNode* node = _model->GetData(postion);
				node->SetType(CNode::NODE_MOVEABLE_BOX);
				emit node->Update(node->GetType(), node->GetPostion());
			}
			else if (_currentBoxMatrix[i][j]->GetType() == CNode::NODE_BLANK_BOX)
			{
				CNode* node = _model->GetData(postion);
				node->SetType(CNode::NODE_BLANK_BOX);
				emit node->Update(node->GetType(), node->GetPostion());
			}

			++postion.column;
		}

		postion.column = _currentBoxPostion.column;
		++postion.row;
	}

	return true;
}

bool CTetrisControl::CreateNextBox()
{
	return true;
}

void CTetrisControl::CreateBlankBoxMatrix(CTetrisModel::NODE_MATRIX& matrix)
{
	CTetrisModel::NODE_MATRIX tetrisNone;
	tetrisNone.resize(4);
	for (int i = 0; i < 4; ++i)
	{
		tetrisNone[i].resize(4);
		for (int j = 0; j < 4; ++j)
		{
			CNode::NODE_POSTION postion;
			postion.row = i;
			postion.column = j;
			tetrisNone[i][j] = new CNode(CNode::NODE_BLANK, postion);
		}
	}

	matrix = tetrisNone;
}

void CTetrisControl::CreateTempMatrix(CTetrisModel::NODE_MATRIX& matrix)
{
	CTetrisModel::NODE_MATRIX tetrisNone;
	tetrisNone.resize(4);
	for (int i = 0; i < 4; ++i)
	{
		tetrisNone[i].resize(4);
	}

	matrix = tetrisNone;
}

void CTetrisControl::InitAllBoxSharp()
{
	CTetrisModel::NODE_MATRIX blankMatrix;
	for (int i = 0; i < sizeof(arrBox) / sizeof(arrBox[0]); ++i)
	{
		if (i % 4 == 0)
		{
			CreateBlankBoxMatrix(blankMatrix);
		}

		for (int j = 0; j < 4; ++j)
		{
			if (arrBox[i][j] == 1)
			{
				blankMatrix[i % 4][j]->SetType(CNode::NODE_MOVEABLE_BOX);
			}
#ifdef TEST
			else if (arrBox[i][j] == 0)
			{
				blankMatrix[i % 4][j]->SetType(CNode::NODE_BLANK_BOX);
			}
#endif // TEST
		}

		if (i % 4 == 3 || i == 26)
		{
			_tetrisAllSharp.push_back(blankMatrix);
		}
	}


	//������������
	//for (int i = 0; i < _model->GetRows(); ++i)
	//{
	//	for (int j = 0; j < _model->GetColumns(); ++j)
	//	{
	//		CNode::NODE_POSTION pos;
	//		pos.row = i;
	//		pos.column = j;
	//		CNode* node = _model->GetData(pos);
	//		emit node->Update(node->GetType(), node->GetPostion());
	//	}
	//}
}

void CTetrisControl::CopyMatrix(CTetrisModel::NODE_MATRIX& from, CTetrisModel::NODE_MATRIX& to)
{
	for (int i = 0; i < from.size(); ++i)
	{
		for (int j = 0; j < from.size(); ++j)
		{
			to[i][j] = from[i][j];
		}
	}
}


void CTetrisControl::RotateMatrix(CTetrisModel::NODE_MATRIX& to, CTetrisModel::NODE_MATRIX& from, ROTATE_DIRECTION direction)
{
	if (direction == ROTATE_DIRECTION::ROTATE_RIGHT)
	{
		for ( int i = 0; i < to.size(); ++i)
		{
			for (int j = 0; j < to.size(); ++j)
			{
				to[i][j] = from[to.size() - j - 1][i];
			}
		}
	}

}

void CTetrisControl::ReleaseMatrix(CTetrisModel::NODE_MATRIX& matrix)
{
	for (int i = 0; i < matrix.size(); ++i)
	{
		for (int j = 0; j < matrix.size(); ++j)
		{
			delete[] matrix[i][j];
		}
	}
}

void CTetrisControl::EreaseCurrentBoxRegion(CNode::NODE_POSTION& currentPos)
{
	CNode::NODE_POSTION pos = currentPos;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (_model->IsValidPostion(pos))
			{
				CNode* node = _model->GetData(pos);
				if (node->GetType() != CNode::NODE_WALL && node->GetType() != CNode::NODE_STATIC_BOX)
				{
					node->SetType(CNode::NODE_BLANK);
					emit node->Update(node->GetType(), node->GetPostion());
				}
			}

			++pos.column;
		}

		pos.column = currentPos.column;
		++pos.row;
	}
}

void CTetrisControl::UpdateCurrentBoxNextRegion(CNode::NODE_POSTION& nextPos, CTetrisModel::NODE_MATRIX& currentBox)
{
	CNode::NODE_POSTION pos = nextPos;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (currentBox[i][j]->GetType() == CNode::NODE_MOVEABLE_BOX)
			{
				CNode* node = _model->GetData(pos);
				if (node)
				{
					node->SetType(CNode::NODE_MOVEABLE_BOX);
					emit node->Update(node->GetType(), node->GetPostion());
				}
			}
			else if (currentBox[i][j]->GetType() == CNode::NODE_BLANK_BOX)
			{
				CNode* node = _model->GetData(pos);
				if (node)
				{
					if (node->GetType() != CNode::NODE_WALL && 
						node->GetType() != CNode::NODE_STATIC_BOX)
					{
						node->SetType(CNode::NODE_BLANK_BOX);
						emit node->Update(node->GetType(), node->GetPostion());
					}
				}
			}
			++pos.column;
		}

		pos.column = nextPos.column;
		++pos.row;
	}
}