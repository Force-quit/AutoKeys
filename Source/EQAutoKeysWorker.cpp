#include "../Headers/EQAutoKeysWorker.h"
#include <QTimer>

#pragma warning(disable:5050)
import eutilities;

int EQAutoKeysWorker::clickHoldTime() const
{
	return mPressHoldTime;
}

int EQAutoKeysWorker::clickInterval() const
{
	return mPressInterval;
}

void EQAutoKeysWorker::setTargetKeys(std::span<const eutilities::Key> iTargetKeys)
{
	mTargetKeys = std::vector<eutilities::Key>(iTargetKeys.begin(), iTargetKeys.end());
}

void EQAutoKeysWorker::setPressInterval(int iInterval)
{
	mPressInterval = iInterval;
}

void EQAutoKeysWorker::setKeysHoldTime(int iHoldTime)
{
	mPressHoldTime = iHoldTime;
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
	if (!mIsActive)
	{
		return;
	}

	for (auto i : mTargetKeys)
	{
		eutilities::pressKey(i);
	}

	QTimer::singleShot(mPressHoldTime, this, &EQAutoKeysWorker::releaseKeys);
}

void EQAutoKeysWorker::releaseKeys()
{
	for (auto i : mTargetKeys)
	{
		eutilities::releaseKey(i);
	}

	QTimer::singleShot(mPressInterval, this, &EQAutoKeysWorker::pressKeys);
}