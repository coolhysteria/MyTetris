#include "stdafx.h"
#include "MyTetris.h"

MyTetris::MyTetris(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

MyTetris::MyTetris(CTetrisModel* model)
{
	ui.setupUi(this);

	_model = model;
	_model->SetBackgound();
	_model->SetWall(1,1,1,1);

	_control.SetModel(model);


	for (int i = 0; i < _model->GetRows(); ++i)
	{
		for (int j = 0; j < _model->GetColumns(); ++j)
		{
			CNode::NODE_POSTION postion;
			postion.row = i;
			postion.column = j;
			CNode* node = _model->GetData(postion);
			if (node)
			{
				connect(
					node,
					SIGNAL(Update(CNode::NODE_TYPE, CNode::NODE_POSTION)),
					this,
					SLOT(OnUpdate(CNode::NODE_TYPE, CNode::NODE_POSTION)));
			}
		}
	}

	_control.Start();
	_timeMoveDown = startTimer(600);

}

void MyTetris::OnUpdate(CNode::NODE_TYPE type, CNode::NODE_POSTION postion)
{
	qDebug("<MyTetris>OnUpdate row:%d column:%d\r\n", postion.row, postion.column);
	_postion = postion;
	_type = type;
	update();
	
}

void MyTetris::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	const int BLOCK_SIZE = 35; //单个方块单元的边长  
	const int MARGIN = 5; //场景边距  
	const int AREA_ROW = 22; //场景行数  
	const int AREA_COL = 14; //场景列数 

	painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
	painter.drawRect(MARGIN, MARGIN, AREA_COL * BLOCK_SIZE, AREA_ROW * BLOCK_SIZE);

	for (int i = 0; i < AREA_ROW; i++)
	{
		for (int j = 0; j < AREA_COL; j++)
		{
			CNode::NODE_POSTION pos;
			pos.row = i;
			pos.column = j;
			CNode* node = _model->GetData(pos);
			if (node)
			{
				if (node->GetType() == CNode::NODE_WALL)
				{
					painter.setBrush(QBrush(Qt::gray, Qt::SolidPattern));
					painter.drawRect(j * BLOCK_SIZE + MARGIN, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
				}
				else if (node->GetType() == CNode::NODE_MOVEABLE_BOX)
				{
					painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
					painter.drawRect(j * BLOCK_SIZE + MARGIN, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
				}
				else if (node->GetType() == CNode::NODE_STATIC_BOX)
				{
					painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
					painter.drawRect(j * BLOCK_SIZE + MARGIN, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
				}
				else if (node->GetType() == CNode::NODE_BLANK_BOX)
				{
					painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
					painter.drawRect(j * BLOCK_SIZE + MARGIN, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
				}


			}


		}
	}

	qDebug("<MyTetris>paintEvent\r\n");
}

void MyTetris::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == _timeMoveDown)
	{
		//_control.Rotate(CTetrisControl::ROTATE_RIGHT);
		_control.MoveDown();
	}
}

void MyTetris::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Down:
	{
		_control.MoveDown();
		break;
	}

	case Qt::Key_Up:
	{
		_control.Rotate(CTetrisControl::ROTATE_DIRECTION::ROTATE_RIGHT);
		break;
	}

	case Qt::Key_Left:
	{
		_control.MoveLeft();
		break;
	}

	case Qt::Key_Right:
	{
		_control.MoveRight();
		break;
	}

	case Qt::Key_Control:
	{
		if (_control.IsStart())
		{
			_control.Stop();
		}
		else
		{
			_control.Restart();
		}
		
		break;
	}

	case Qt::Key_Space:
	{
		_control.MoveDownFast();

		break;
	}


	}
}