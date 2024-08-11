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
	QGroupBox* initActivationLayout();

	QHBoxLayout* initKeysHoldTimeLayout();
	QHBoxLayout* initPressIntervalLayout();
	QHBoxLayout* initActivationDelayLayout();

	EQAutoKeysWorker* mAutoKeysWorker{ new EQAutoKeysWorker };
	QThread mWorkerThread;

	EQShortcutPicker* mShortcutPicker{ new EQShortcutPicker("Activation shortcut :") };
	EQShortcutPicker* mTargetKeysPicker{ new EQShortcutPicker("Target keys :") };

	EQIntLineEdit* mKeysHoldTimeEdit{};
	EQIntLineEdit* mPressIntervalEdit{};
	EQIntLineEdit* mActivationDelayEdit{};

	QLabel* mActivationStatusText{};

	bool mIsActive{};
};