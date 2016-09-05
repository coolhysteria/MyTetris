#ifndef CBOX_H
#define CBOX_H

#include <QObject>
#include "TetrisModel.h"


static  char arrayBoxes[][4] =
{
	0,0,0,0,
	1,1,1,1,
	0,0,0,0,
	0,0,0,0,

	1,1,0,0,
	1,1,0,0,
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

	1,1,0,0,
	0,1,0,0,
	0,1,0,0,
	0,0,0,0,

	1,0,0,0,
	1,1,0,0,
	0,1,0,0,
	0,0,0,0,

	0,1,0,0,
	1,1,0,0,
	1,0,0,0,
	0,0,0,0
};


class CBox : public QObject
{
	Q_OBJECT

public:
	enum BOX_SHARP
	{
		//L
		BOX_UP_L = 0x01,
		BOX_DOWN_L,
		BOX_LEFT_L,
		BOX_RIGHT_L,

		//I
		BOX_UP_I,
		BOX_DOWN_I,
		BOX_LEFT_I,
		BOX_RIGHT_I,

		//Z
		BOX_UP_Z,
		BOX_DOWN_Z,
		BOX_LEFT_Z,
		BOX_RIGHT_Z,

		//Ìï
		BOX_UP_CROSS,
		BOX_DOWN_CROSS,
		BOX_LEFT_CROSS,
		BOX_RIGHT_CROSS,

		//T
		BOX_UP_T,
		BOX_DOWN_T,
		BOX_LEFT_T,
		BOX_RIGHT_T,

	};


	struct TETRIS_BOX
	{
		BOX_SHARP sharp;
		CTetrisModel::NODE_MATRIX matrix;
	};

	CBox(QObject *parent);
	CBox(enum BOX_SHARP sharp);
	~CBox();


private:
	static vector<TETRIS_BOX> _tetrisAllSharp;

	
};

#endif // CBOX_H
