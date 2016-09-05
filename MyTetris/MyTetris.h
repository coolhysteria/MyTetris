#pragma once

#include <QtWidgets/QWidget>
#include "ui_MyTetris.h"
#include "TetrisModel.h"
#include "TetrisControl.h"

class MyTetris : public QWidget
{
    Q_OBJECT

public:
    MyTetris(QWidget *parent = Q_NULLPTR);
	MyTetris(CTetrisModel* model);

	public slots:
	void OnUpdate(CNode::NODE_TYPE type, CNode::NODE_POSTION postion);

protected:
	void paintEvent(QPaintEvent *event);
	void timerEvent(QTimerEvent *event);
	void keyPressEvent(QKeyEvent *event);

private:
    Ui::MyTetrisClass ui;
	CTetrisModel* _model;
	CTetrisControl _control;
	int _timeMoveDown;
	int _timeNew;

	CNode::NODE_POSTION _postion;
	CNode::NODE_TYPE _type;
};
