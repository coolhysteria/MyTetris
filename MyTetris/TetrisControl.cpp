#include "stdafx.h"
#include "TetrisControl.h"
#include <QSoundEffect>  
static CNode::NODE_SHARP arrSharps[] = 
{
	CNode::NODE_SHARP::NODE_SHARP_O,
	CNode::NODE_SHARP::NODE_SHARP_I,
	CNode::NODE_SHARP::NODE_SHARP_T,
	CNode::NODE_SHARP::NODE_SHARP_L,
	CNode::NODE_SHARP::NODE_SHARP_Z,
	CNode::NODE_SHARP::NODE_SHARP_Z,
	CNode::NODE_SHARP::NODE_SHARP_L
};


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
	effect.setSource(QUrl::fromLocalFile("./wav/back.wav"));
	effect.setLoopCount(QSoundEffect::Infinite);
	effect.setVolume(0.6f);
	effect.play();


	_bgmPlayerGameStart.Init();
	_bgmPlayerRotateBox.Init();
	_bgmPlayerFixBox.Init();
	_bgmPlayerReleaseBox.Init();

	_bgmPlayerGameStart.PlayWav(QString("./wav/ReadyGo.WAV"));


	InitAllBoxSharp();
	
	CreateNewBox();

	_isStarted = true;


	return true;
}

bool CTetrisControl::IsStart() const
{
	return _isStarted;
}

bool CTetrisControl::SetStartBGMWavePath(char* path)
{
	return true;
}

bool CTetrisControl::SetMoveBGMWavePath(char* path)
{
	return true;
}

bool CTetrisControl::SetFixedBGMWavePath(char* path)
{
	return true;
}

bool CTetrisControl::ReleasedOverBGMWavePath(char* path)
{
	return true;
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

	//_bgmPlayerRotateBox.PlayWav(QString("./wav/transform.wav"));

	CNode::NODE_POSTION nextPostion;
 	nextPostion = _currentBoxPostion;
	nextPostion.column -= 1;


	//判断下个位置是否可以移动
	if (CanMove(nextPostion, _currentBoxMatrix) == false)
	{
		return false;
	}

	//擦除当前区域
	EreaseCurrentBoxRegion(_currentBoxPostion);

	//更新新区域
	UpdateCurrentBoxNextRegion(nextPostion, _currentBoxMatrix);

	//更新当前位置
	_currentBoxPostion = nextPostion;

	return true;
}

bool CTetrisControl::MoveRight()
{
	if (_isStarted == false)
	{
		return true;
	}

	//_bgmPlayerRotateBox.PlayWav(QString("./wav/transform.wav"));

	CNode::NODE_POSTION nextPostion;
	nextPostion = _currentBoxPostion;
	nextPostion.column += 1;


	//判断下个位置是否可以移动
	if (CanMove(nextPostion, _currentBoxMatrix) == false)
	{
		return false;
	}

	//擦除当前区域
	EreaseCurrentBoxRegion(_currentBoxPostion);

	//更新新区域
	UpdateCurrentBoxNextRegion(nextPostion, _currentBoxMatrix);

	//更新当前位置
	_currentBoxPostion = nextPostion;

	return true;
}

bool CTetrisControl::MoveDown()
{
	if (_isStarted == false)
	{
		return true;
	}

	//_bgmPlayerRotateBox.PlayWav(QString("./wav/transform.wav"));

	CNode::NODE_POSTION nextPostion;
	nextPostion = _currentBoxPostion;
	nextPostion.row += 1;


	//判断下个位置是否可以移动
	if (CanMove(nextPostion, _currentBoxMatrix) == false)
	{
		//固定方块
		FixBox(_currentBoxPostion, _currentBoxMatrix);

		//消除行
		ReleaseLines(_currentBoxPostion, _currentBoxMatrix);

		//创建新方块
		CreateNewBox();

		return false;
	}

	//擦除当前区域
	EreaseCurrentBoxRegion(_currentBoxPostion);

	//更新新区域
	UpdateCurrentBoxNextRegion(nextPostion, _currentBoxMatrix);

	//更新当前位置
	_currentBoxPostion = nextPostion;

	return true;
}

bool CTetrisControl::Rotate(ROTATE_DIRECTION direction)
{
	if (_isStarted == false)
	{
		return true;
	}

	_bgmPlayerRotateBox.PlayWav(QString("./wav/transform.wav"));

	//复制当前矩阵
	CTetrisModel::NODE_MATRIX copidMatrix;
	CreateTempMatrix(copidMatrix);
	CopyMatrix(_currentBoxMatrix, copidMatrix);

	//旋转矩阵
	RotateMatrix(copidMatrix, _currentBoxMatrix, direction);


	//判断新矩阵是否可以移动
	if (CanMove(_currentBoxPostion, copidMatrix) == false)
	{
		return false;
	}


	//更新当前矩阵
	CopyMatrix(copidMatrix, _currentBoxMatrix);

	//擦除当前区域
	EreaseCurrentBoxRegion(_currentBoxPostion);

	//更新新区域
	UpdateCurrentBoxNextRegion(_currentBoxPostion, _currentBoxMatrix);

	return true;
}

bool CTetrisControl::MoveDownFast()
{
	return true;
}

bool CTetrisControl::CanMove(CNode::NODE_POSTION& nextPostion, CTetrisModel::NODE_MATRIX& currentBox) const
{
	//box为NODE_MOVEABLE_BOX的时候，背景为NODE_BLANK
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

	//将NODE_MOVEABLE_BOX修改为NODE_STATIC_BOX
	for (int i = 0; i < currentBox.size(); ++i)
	{
		for (int j = 0; j < currentBox.size(); ++j)
		{
			if (_model->IsValidPostion(posCur))
			{
				//NODE_MOVEABLE_BOX改成NODE_STATIC_BOX
				if (currentBox[i][j]->GetType() == CNode::NODE_MOVEABLE_BOX)
				{
					CNode* node = _model->GetData(posCur);
					node->SetType(CNode::NODE_STATIC_BOX);
					node->SetBelongSharp(currentBox[i][j]->GetBelongSharp());
					emit node->Update(node->GetType(), node->GetBelongSharp(), node->GetPostion());
					//增加当前行的NODE_STATIC_BOX数量
					_lineStaticBoxCounts[posCur.row] += 1;

				}
				//NODE_BLANK_BOX不是NODE_WALL NODE_STATIC_BOX改成NODE_BLANK
				else if (currentBox[i][j]->GetType() == CNode::NODE_BLANK_BOX)
				{
					if (_model->GetData(posCur)->GetType() != CNode::NODE_WALL && 
						_model->GetData(posCur)->GetType() != CNode::NODE_STATIC_BOX)
					{
						CNode* node = _model->GetData(posCur);
						node->SetType(CNode::NODE_BLANK);
						emit node->Update(node->GetType(), node->GetBelongSharp(), node->GetPostion());
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
	//将一行占据满了的消除
	int totalFullLines = 0;
	int releaseCounts = _model->GetColumns() - _model->GetLeftWidth() - _model->GetRightWidth();
	int maxRowCounts = _model->GetRows();
	for (int iRow = postion.row; iRow < maxRowCounts; ++iRow)
	{
		//如果达到消除数量
		if (_lineStaticBoxCounts[iRow] == releaseCounts)
		{
			++totalFullLines;
			ReleaseLine(iRow);
		}
	}

	if (totalFullLines > 0)
	{
		//_bgmPlayerFixBox.PlayWav(QString("./wav/fadelayer.wav"));
	}
	else
	{
		_bgmPlayerReleaseBox.PlayWav(QString("./wav/fixup.wav"));
	}

	return totalFullLines;
}

bool CTetrisControl::ReleaseLine(int row)
{
	int iRow = row;
	//向上遍历直到当前行没有NODE_STATIC_BOX
	while (_lineStaticBoxCounts[iRow] > 0)
	{
		for (int iColumn = 0; iColumn < _model->GetColumns(); ++iColumn)
		{	
			//计算当前行的位置
			CNode::NODE_POSTION currentPos;
			currentPos.column = iColumn;
			currentPos.row = iRow;

			//获取上一行的类型
			CNode::NODE_POSTION beforeRowPos = currentPos;
			--beforeRowPos.row;
			CNode::NODE_TYPE typeBeforeRow = _model->GetData(beforeRowPos)->GetType();
			CNode::NODE_SHARP sharpBeforeRow = _model->GetData(beforeRowPos)->GetBelongSharp();

			//将当前行设置为上一行
			CNode* node = _model->GetData(currentPos);
			node->SetType(typeBeforeRow);
			node->SetBelongSharp(sharpBeforeRow);
			emit node->Update(node->GetType(), node->GetBelongSharp(), node->GetPostion());

		}

		_lineStaticBoxCounts[iRow] = _lineStaticBoxCounts[iRow - 1];

		--iRow;
	}

	_bgmPlayerFixBox.PlayWav(QString("./wav/fadelayer.wav"));

	return true;
}

bool CTetrisControl::CreateNewBox()
{

	int rows = _model->GetRows();
	int columns = _model->GetColumns();


	//第二行，第二列
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
				node->SetBelongSharp(_currentBoxMatrix[i][j]->GetBelongSharp());
				emit node->Update(node->GetType(), node->GetBelongSharp(), node->GetPostion());
			}
			else if (_currentBoxMatrix[i][j]->GetType() == CNode::NODE_BLANK_BOX)
			{
				CNode* node = _model->GetData(postion);
				node->SetType(CNode::NODE_BLANK_BOX);
				emit node->Update(node->GetType(), node->GetBelongSharp(), node->GetPostion());
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
	int sharpIndex = -1;
	for (int i = 0; i < sizeof(arrBox) / sizeof(arrBox[0]); ++i)
	{
		if (i % 4 == 0)
		{
			++sharpIndex;
			CreateBlankBoxMatrix(blankMatrix);
		}

		for (int j = 0; j < 4; ++j)
		{
			if (arrBox[i][j] == 1)
			{
				//设置方块类型和形状
				blankMatrix[i % 4][j]->SetType(CNode::NODE_MOVEABLE_BOX);
				blankMatrix[i % 4][j]->SetBelongSharp(arrSharps[sharpIndex]);
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




	_lineStaticBoxCounts.clear();
	for (int i = 0; i < _model->GetRows(); ++i)
	{
		_lineStaticBoxCounts.push_back(0);
	}


	//遍历背景数组
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

void CTetrisControl::RotateMatrixEx(CTetrisModel::NODE_MATRIX& to, CTetrisModel::NODE_MATRIX& from, ROTATE_DIRECTION direction)
{

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
					node->SetBelongSharp(CNode::NODE_SHARP_NONE);
					emit node->Update(node->GetType(), node->GetBelongSharp(), node->GetPostion());
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
					node->SetBelongSharp(currentBox[i][j]->GetBelongSharp());
					emit node->Update(node->GetType(), node->GetBelongSharp(), node->GetPostion());
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
						emit node->Update(node->GetType(), node->GetBelongSharp(), node->GetPostion());
					}
				}
			}
			++pos.column;
		}

		pos.column = nextPos.column;
		++pos.row;
	}
}