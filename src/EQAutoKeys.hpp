#pragma once

#include "EQAutoKeysWorker.hpp"
#include <EQIntLineEdit.hpp>
#include <EQShortcutPicker.hpp>
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