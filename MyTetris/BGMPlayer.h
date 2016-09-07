#pragma once 
#include <QObject>
#include <QMultimedia>  
#include <QString>
#include <QAudioOutput>

class CBGMPlayer : public QObject
{
	Q_OBJECT

public:
	CBGMPlayer(QObject* parent = 0);
	~CBGMPlayer();

	void Init();
	void PlayWav(QString wav);

private:
	QAudioOutput* _audioRotate;
	QAudioFormat _format;
	QFile _sourceFile;
};

