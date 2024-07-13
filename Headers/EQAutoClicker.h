#pragma once

#include "EQAutoClickerWorker.h"
#include <EQUtilities/EQIntLineEdit.h>
#include <EQUtilities/EQShortcutPicker.h>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QString>
#include <QThread>
#include <QVector>
#include <QWidget>

class EQAutoClicker : public QMainWindow
{
	Q_OBJECT

public:
	EQAutoClicker();
	~EQAutoClicker();

private slots:
	void disableWidgets();
	void enableWidgets();

private:
	QGroupBox* initParameters();
	QHBoxLayout* initClickHoldTime();
	QHBoxLayout* initClicksInterval();
	QHBoxLayout* initClickButton();
	QGroupBox* initActivationLayout();

	EQAutoClickerWorker* mInputRecorderWorker{ new EQAutoClickerWorker };
	QThread mWorkerThread;

	EQShortcutPicker* mShortcutPicker{};
	EQIntLineEdit* mClickHoldTimeEdit{};
	EQIntLineEdit* mClickIntervalEdit{};
	QRadioButton* mLeftClickButton{};
	QRadioButton* mRightClickButton{};
	QLabel* mActivationStatusText{};
	QVector<QWidget*> mWidgetsToDisable;
};