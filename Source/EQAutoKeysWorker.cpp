#include "../Headers/EQAutoKeysWorker.h"
#include <QTimer>

#pragma warning(disable:5050)
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
	pressKeys();
}

void EQAutoKeysWorker::stop()
{
	mIsActive = false;
}

void EQAutoKeysWorker::pressKeys()
{
	if (mIsActive)
	{
		for (auto i : mTargetKeys)
		{
			eutilities::pressKey(i);
		}
		QTimer::singleShot(mClickHoldTime, this, &EQAutoKeysWorker::releaseKeys);
	}
}

void EQAutoKeysWorker::releaseKeys()
{
	for (auto i : mTargetKeys)
	{
		eutilities::releaseKey(i);
	}
	QTimer::singleShot(mClickInterval, this, &EQAutoKeysWorker::pressKeys);
}