#pragma once

#include <QObject>
#include <span>
#include <vector>

#pragma warning(disable:5050)
import eutilities;

class EQAutoKeysWorker : public QObject
{
	Q_OBJECT

public:
	static constexpr int MIN_INTERVAL{ 1 };
	static constexpr int MAX_INTERVAL{ std::numeric_limits<int>::max() };
	static constexpr int DEFAULT_HOLD_TIME{ 10 };
	static constexpr int DEFAULT_BETWEEN_TIME{ 30 };

	int clickHoldTime() const;
	int clickInterval() const;

	void setTargetKeys(std::span<const eutilities::Key> iTargetKeys);

public slots:
	void setKeysHoldTime(int iHoldTime);
	void setPressInterval(int iInterval);
	void start();
	void stop();

private slots:
	void pressKeys();
	void releaseKeys();

private:
	std::vector<eutilities::Key> mTargetKeys;
	int mPressHoldTime{ DEFAULT_HOLD_TIME };
	int mPressInterval{ DEFAULT_BETWEEN_TIME };
	bool mIsActive{};
};