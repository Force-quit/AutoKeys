#pragma once

#include <QObject>
#include <span>
#include <vector>
#include <EUtilities-Windows.hpp>

class EQAutoKeysWorker : public QObject
{
	Q_OBJECT

public:
	static constexpr int MIN_INTERVAL{ 1 };
	static constexpr int MAX_INTERVAL{ 0x0FFFFFFF };
	static constexpr int DEFAULT_HOLD_TIME{ 10 };
	static constexpr int DEFAULT_BETWEEN_TIME{ 30 };
	static constexpr int DEFAULT_ACTIVATION_DELAY{ 50 };

	void setTargetKeys(std::span<const eutilities::Key> iTargetKeys);

public slots:
	void setKeysHoldTime(int iHoldTime);
	void setPressInterval(int iInterval);
	void setActivationDelay(int iActivationDelay);
	void start();
	void stop();

private slots:
	void pressKeys(std::int8_t iActivationCount);
	void releaseKeys();

private:
	std::vector<eutilities::Key> mTargetKeys;
	int mPressHoldTime{ DEFAULT_HOLD_TIME };
	int mPressInterval{ DEFAULT_BETWEEN_TIME };
	int mActivationDelay{ DEFAULT_ACTIVATION_DELAY };
	bool mIsActive{};
	std::int8_t mActivationCount{};
};