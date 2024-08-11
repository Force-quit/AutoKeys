#include "../Headers/EQAutoKeysWorker.h"
#include <QTimer>

#pragma warning(disable:5050)
import eutilities;

void EQAutoKeysWorker::setTargetKeys(std::span<const eutilities::Key> iTargetKeys)
{
	mTargetKeys = std::vector<eutilities::Key>(iTargetKeys.begin(), iTargetKeys.end());
}

void EQAutoKeysWorker::setPressInterval(int iInterval)
{
	mPressInterval = iInterval;
}

void EQAutoKeysWorker::setActivationDelay(int iActivationDelay)
{
	mActivationDelay = iActivationDelay;
}

void EQAutoKeysWorker::setKeysHoldTime(int iHoldTime)
{
	mPressHoldTime = iHoldTime;
}

void EQAutoKeysWorker::start()
{
	mIsActive = true;
	++mActivationCount;
	eutilities::sleepFor(mActivationDelay);
	pressKeys(mActivationCount);
}

void EQAutoKeysWorker::stop()
{
	mIsActive = false;
}

void EQAutoKeysWorker::pressKeys(std::int8_t iActivationCount)
{
	if (!mIsActive || iActivationCount != mActivationCount)
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

	QTimer::singleShot(mPressInterval, std::bind_front(&EQAutoKeysWorker::pressKeys, this, mActivationCount));
}