#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QString>
#include "EQAutoClickerWorker.h"
#include <QThread>
#include <QRadioButton>
#include <QPushButton>
#include <QVector>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <EQUtilities/EQIntLineEdit.h>
#include <EQUtilities/EQShortcutListener.h>


class EQAutoClicker : public QMainWindow
{
	Q_OBJECT

public:
	EQAutoClicker(QWidget *parent = nullptr);
	~EQAutoClicker();

private slots:
	void disableWidgets();
	void enableWidgets();
	void saveConfiguration();
	void loadConfiguration();

private:
	EQAutoClickerWorker* worker;
	QThread workerThread;

	EQShortcutListener* shortcutListener;
	EQIntLineEdit* clickHoldTimeEdit;
	EQIntLineEdit* timeBetweenClicksEdit;
	QRadioButton* leftClickButton;
	QRadioButton* rightClickButton;
	QPushButton* saveButton;
	QPushButton* loadButton;
	QLabel* configurationText;
	QLabel* activationStatusText;
	QVector<QWidget*> widgetsToDisable;

	QGroupBox* initParameters();
	QHBoxLayout* initClickHoldTime();
	QHBoxLayout* initTimeBetweenClicks();
	QHBoxLayout* initClickButton();
	QHBoxLayout* initSaveAndLoad();

	QGroupBox* initActivationLayout();

};
