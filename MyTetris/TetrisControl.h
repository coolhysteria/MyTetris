#pragma once
#include <QObject>
#include "TetrisModel.h"
#include "Node.h"
#include "BGMPlayer.h"
#include "QSoundEffect"

class CTetrisControl : public QObject
{
	Q_OBJECT

public:
	enum ROTATE_DIRECTION
	{
		ROTATE_LEFT = 0x01,
		ROTATE_RIGHT
	};

	CTetrisControl(void);
	~CTetrisControl(void);

	bool Start();
	void Stop();
	void Restart();
	bool IsStart() const;

	bool SetStartBGMWavePath(char* path);
	bool SetMoveBGMWavePath(char* path);
	bool SetFixedBGMWavePath(char* path);
	bool ReleasedOverBGMWavePath(char* path);

	void SetModel(CTetrisModel *model);

	

signals:
	//开始背景音乐
	void StartBGM();
	void MoveBGM();
	void FixedBGM();
	void ReleasedOverBGM();


public slots:
	//左移
	bool MoveLeft();

	//右移
	bool MoveRight();

	//下移
	bool MoveDown();

	//旋转
	bool Rotate(ROTATE_DIRECTION direction);

	//迅速下降
	bool MoveDownFast();

	//创建新方块
	bool CreateNewBox();
	
	//创建下一个方块
	bool CreateNextBox();

private:
	CNode::NODE_POSTION _currentBoxPostion;
	CTetrisModel::NODE_MATRIX _currentBoxMatrix;
	CTetrisModel::NODE_MATRIX _nextBox;

	CTetrisModel* _model;
	vector<CTetrisModel::NODE_MATRIX> _tetrisAllSharp;
	vector<int> _lineStaticBoxCounts;

	bool _isStarted;

	CBGMPlayer _bgmPlayerBackgound;
	CBGMPlayer _bgmPlayerGameStart;
	CBGMPlayer _bgmPlayerRotateBox;
	CBGMPlayer _bgmPlayerFixBox;
	CBGMPlayer _bgmPlayerReleaseBox;
	QSoundEffect effect;

	void CreateBlankBoxMatrix(CTetrisModel::NODE_MATRIX& matrix);
	void CreateTempMatrix(CTetrisModel::NODE_MATRIX& matrix);
	void InitAllBoxSharp();

	void CopyMatrix(CTetrisModel::NODE_MATRIX& from, CTetrisModel::NODE_MATRIX& to);
	void RotateMatrix(CTetrisModel::NODE_MATRIX& to, CTetrisModel::NODE_MATRIX& from, ROTATE_DIRECTION direction);
	void RotateMatrixEx(CTetrisModel::NODE_MATRIX& to, CTetrisModel::NODE_MATRIX& from, ROTATE_DIRECTION direction);
	void ReleaseMatrix(CTetrisModel::NODE_MATRIX& matrix);


	void EreaseCurrentBoxRegion(CNode::NODE_POSTION& currentPos);
	void UpdateCurrentBoxNextRegion(CNode::NODE_POSTION& nextPos, CTetrisModel::NODE_MATRIX& currentBox);

	bool CanMove(CNode::NODE_POSTION& nextPostion, CTetrisModel::NODE_MATRIX& currentBox) const;
	void FixBox(CNode::NODE_POSTION& postion, CTetrisModel::NODE_MATRIX& currentBox);
	int ReleaseLines(CNode::NODE_POSTION& postion, CTetrisModel::NODE_MATRIX& currentBox);
	bool ReleaseLine(int row);

};

