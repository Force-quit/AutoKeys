#include "../Headers/EQAutoKeysWorker.h"
#include <QTimer>
import eutilities;

void EQAutoKeysWorker::switchState()
{
	mActive = !mActive;
	if (mActive)
	{
		emit activated();
		clickDown();
	}
	else
	{
		emit deactivated();
	}
}

void EQAutoKeysWorker::clickDown()
{
	if (mActive)
	{
		eutilities::pressKey(mLeftClick ? eutilities::LEFT_CLICK : eutilities::RIGHT_CLICK);
		QTimer::singleShot(mClickHoldTime, this, &EQAutoKeysWorker::clickUp);
	}
}

void EQAutoKeysWorker::clickUp()
{
	eutilities::releaseKey(mLeftClick ? eutilities::LEFT_CLICK : eutilities::RIGHT_CLICK);
	QTimer::singleShot(mClickInterval, this, &EQAutoKeysWorker::clickDown);
}