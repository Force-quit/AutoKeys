#pragma once

#include <QObject>

class EQAutoClickerWorker : public QObject
{
	Q_OBJECT

public:
	EQAutoClickerWorker() = default;

	static constexpr int MIN_INTERVAL{ 1 };
	static constexpr int MAX_INTERVAL{ std::numeric_limits<int>::max() };
	static constexpr int DEFAULT_HOLD_TIME{ 10 };
	static constexpr int DEFAULT_BETWEEN_TIME{ 30 };

	inline int clickHoldTime() const { return mClickHoldTime; }
	inline int clickInterval() const { return mClickInterval; }
	inline bool isTargetLeftClick() const { return mLeftClick; }
	inline bool isActive() const { return mActive; }
	inline void setLeftClick(bool iLeftClick) { mLeftClick = iLeftClick; }

public slots:
	inline void setClickHoldTime(int iHoldTime) { mClickHoldTime = iHoldTime; }
	inline void setClickInterval(int iInterval) { mClickInterval = iInterval; }
	void switchState();

signals:
	void activated();
	void deactivated();

private slots:
	void clickDown();
	void clickUp();

private:
	int mClickHoldTime{ DEFAULT_HOLD_TIME };
	int mClickInterval{ DEFAULT_BETWEEN_TIME };
	bool mLeftClick{ true };
	bool mActive{};
};