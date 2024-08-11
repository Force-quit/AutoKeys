#pragma once

#include "EQAutoKeysWorker.h"
#include <EQUtilities/EQIntLineEdit.h>
#include <EQUtilities/EQShortcutPicker.h>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QThread>
#include <QWidget>

class EQAutoKeys : public QMainWindow
{
	Q_OBJECT

public:
	EQAutoKeys();
	~EQAutoKeys();

private slots:
	void disableWidgets();
	void enableWidgets();
	void switchState();
	void setTargetKeys();

private:
	QGroupBox* initParameters();
	QHBoxLayout* initKeysHoldTime();
	QHBoxLayout* initPressInterval();
	QGroupBox* initActivationLayout();

	EQAutoKeysWorker* mAutoKeysWorker{ new EQAutoKeysWorker };
	QThread mWorkerThread;

	EQShortcutPicker* mShortcutPicker{};
	EQShortcutPicker* mTargetKeysPicker{};

	EQIntLineEdit* mKeysHoldTimeEdit{};
	EQIntLineEdit* mPressIntervalEdit{};
	QLabel* mActivationStatusText{};

	bool mIsActive{};
};