#include "stdafx.h"
#include "MyTetris.h"


const int BLOCK_SIZE = 35; //�������鵥Ԫ�ı߳�  
const int MARGIN = 5; //�����߾�  
const int AREA_ROW = 22; //��������  
const int AREA_COL = 14; //�������� 

static QColor arrSharpsColor[] =
{
	Qt::red,
	Qt::green,
	Qt::blue,
	Qt::yellow,
	Qt::darkRed,
	Qt::darkGreen,
	Qt::darkYellow
};

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
					SIGNAL(Update(CNode::NODE_TYPE, CNode::NODE_SHARP, CNode::NODE_POSTION)),
					this,
					SLOT(OnUpdate(CNode::NODE_TYPE, CNode::NODE_SHARP, CNode::NODE_POSTION)));
			}
		}
	}


	_control.Start();
	_timeMoveDown = startTimer(500);
}


void MyTetris::OnUpdate(CNode::NODE_TYPE type, CNode::NODE_SHARP sharp, CNode::NODE_POSTION postion)
{
	QRect rect(
		QPoint(MARGIN + BLOCK_SIZE * postion.column, MARGIN + BLOCK_SIZE * postion.row),
		QPoint(MARGIN + BLOCK_SIZE * (postion.column + 1), MARGIN + BLOCK_SIZE * (postion.row + 1))
	);

	update(rect);
	
}

void MyTetris::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	painter.setRenderHint(QPainter::HighQualityAntialiasing);
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
					painter.setBrush(QBrush(arrSharpsColor[node->GetBelongSharp()], Qt::SolidPattern));
					painter.drawRect(j * BLOCK_SIZE + MARGIN, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);

				}
				else if (node->GetType() == CNode::NODE_STATIC_BOX)
				{
					painter.setBrush(QBrush(arrSharpsColor[node->GetBelongSharp()], Qt::SolidPattern));
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