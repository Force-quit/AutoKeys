#include "../Headers/EQAutoKeysWorker.h"
#include <QTimer>

import eutilities;

int EQAutoKeysWorker::clickHoldTime() const
{
	return mClickHoldTime;
}

int EQAutoKeysWorker::clickInterval() const
{
	return mClickInterval;
}

void EQAutoKeysWorker::setTargetKeys(std::span<const eutilities::Key> iTargetKeys)
{
	mTargetKeys = std::vector<eutilities::Key>(iTargetKeys.begin(), iTargetKeys.end());
}

void EQAutoKeysWorker::setPressInterval(int iInterval)
{
	mClickInterval = iInterval;
}

void EQAutoKeysWorker::setKeysHoldTime(int iHoldTime)
{
	mClickHoldTime = iHoldTime;
}

void EQAutoKeysWorker::start()
{
	mIsActive = true;
	clickDown();
}

void EQAutoKeysWorker::stop()
{
	mIsActive = false;
}

void EQAutoKeysWorker::clickDown()
{
	if (mIsActive)
	{
		//eutilities::pressKey(mLeftClick ? eutilities::LEFT_CLICK : eutilities::RIGHT_CLICK);
		QTimer::singleShot(mClickHoldTime, this, &EQAutoKeysWorker::clickUp);
	}
}

void EQAutoKeysWorker::clickUp()
{
	//eutilities::releaseKey(mLeftClick ? eutilities::LEFT_CLICK : eutilities::RIGHT_CLICK);
	QTimer::singleShot(mClickInterval, this, &EQAutoKeysWorker::clickDown);
}