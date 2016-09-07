#include "stdafx.h"
#include "BGMPlayer.h"


CBGMPlayer::CBGMPlayer(QObject* parent /*= 0*/) :
	QObject(parent)
{
}


CBGMPlayer::~CBGMPlayer()
{
}

void CBGMPlayer::Init()
{
	_format.setSampleRate(44100);
	_format.setChannelCount(1);
	_format.setSampleSize(16);
	_format.setCodec("audio/pcm");
	_format.setByteOrder(QAudioFormat::LittleEndian);
	_format.setSampleType(QAudioFormat::UnSignedInt);

	_audioRotate = new QAudioOutput(_format, this);
}

void CBGMPlayer::PlayWav(QString wav)
{
	_sourceFile.setFileName(wav);
	_sourceFile.open(QIODevice::ReadOnly);
	
	_audioRotate->start(&_sourceFile);
}
