#include "../Headers/EQAutoClickerWorker.h"
#include <QTimer>
import eutilities;

void EQAutoClickerWorker::switchState()
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

void EQAutoClickerWorker::clickDown()
{
	if (mActive)
	{
		eutilities::pressKey(mLeftClick ? eutilities::LEFT_CLICK : eutilities::RIGHT_CLICK);
		QTimer::singleShot(mClickHoldTime, this, &EQAutoClickerWorker::clickUp);
	}
}

void EQAutoClickerWorker::clickUp()
{
	eutilities::releaseKey(mLeftClick ? eutilities::LEFT_CLICK : eutilities::RIGHT_CLICK);
	QTimer::singleShot(mClickInterval, this, &EQAutoClickerWorker::clickDown);
}