#include "../Headers/EQAutoKeys.h"
#include "../Headers/EQAutoKeysWorker.h"
#include <EQUtilities/EQIntLineEdit.h>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFocusEvent>
#include <QGroupBox>
#include <QIcon>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QStandardPaths>
#include <QTabBar>
#include <QVector>

EQAutoKeys::EQAutoKeys()
{
	auto* centralWidget{ new QWidget };
	setCentralWidget(centralWidget);

	auto* centralLayout{ new QVBoxLayout };
	centralWidget->setLayout(centralLayout);
	centralLayout->addWidget(initParameters());
	centralLayout->addWidget(initActivationLayout());

	mShortcutPicker->startListening();
	setWindowIcon(QIcon(":/images/mouse.png"));
}

QGroupBox* EQAutoKeys::initParameters()
{
	auto* wParameters{ new QGroupBox("Parameters") };

	auto* wParametersLayout{ new QVBoxLayout };
	wParameters->setLayout(wParametersLayout);

	mTargetKeysPicker = new EQShortcutPicker("Target keys :");
	wParametersLayout->addWidget(mTargetKeysPicker);
	mTargetKeysPicker->setTargetKeys(std::vector{ eutilities::UNKNOWN });

	wParametersLayout->addLayout(initKeysHoldTime());
	wParametersLayout->addLayout(initPressInterval());

	connect(mTargetKeysPicker, &EQShortcutPicker::startedChangingShortcut, this, &EQAutoKeys::disableWidgets);
	connect(mTargetKeysPicker, &EQShortcutPicker::stoppedChangingShortcut, this, &EQAutoKeys::enableWidgets);
	connect(mTargetKeysPicker, &EQShortcutPicker::stoppedChangingShortcut, this, &EQAutoKeys::setTargetKeys);

	return wParameters;
}

QGroupBox* EQAutoKeys::initActivationLayout()
{
	auto* activationGroupBox{ new QGroupBox("Activation") };

	auto* groupBoxLayout{ new QVBoxLayout };
	activationGroupBox->setLayout(groupBoxLayout);

	mShortcutPicker = new EQShortcutPicker("Activation shortcut :");
	groupBoxLayout->addWidget(mShortcutPicker);

	auto* activationStatusLayout{ new QHBoxLayout };
	groupBoxLayout->addLayout(activationStatusLayout);
	activationStatusLayout->setAlignment(Qt::AlignCenter);

	auto* activationStatusLabel{ new QLabel("Status :") };
	activationStatusLayout->addWidget(activationStatusLabel);

	mActivationStatusText = new QLabel("Innactive");
	activationStatusLayout->addWidget(mActivationStatusText);

	connect(mShortcutPicker, &EQShortcutPicker::startedChangingShortcut, this, &EQAutoKeys::disableWidgets);
	connect(mShortcutPicker, &EQShortcutPicker::stoppedChangingShortcut, this, &EQAutoKeys::enableWidgets);
	connect(mShortcutPicker, &EQShortcutPicker::shortcutPressed, this, &EQAutoKeys::switchState);

	connect(&mWorkerThread, &QThread::finished, mAutoKeysWorker, &QObject::deleteLater);
	mAutoKeysWorker->moveToThread(&mWorkerThread);
	mWorkerThread.start();

	return activationGroupBox;
}

QHBoxLayout* EQAutoKeys::initKeysHoldTime()
{
	auto* clickHoldTimeLayout{ new QHBoxLayout };

	auto* title{ new QLabel("Keys hold time :") };
	clickHoldTimeLayout->addWidget(title);

	mKeysHoldTimeEdit = new EQIntLineEdit(EQAutoKeysWorker::MIN_INTERVAL, EQAutoKeysWorker::MAX_INTERVAL, EQAutoKeysWorker::DEFAULT_HOLD_TIME);
	clickHoldTimeLayout->addWidget(mKeysHoldTimeEdit);

	auto* unitsLabel{ new QLabel("ms") };
	clickHoldTimeLayout->addWidget(unitsLabel);

	connect(mKeysHoldTimeEdit, &EQIntLineEdit::valueChanged, mAutoKeysWorker, &EQAutoKeysWorker::setKeysHoldTime);

	return clickHoldTimeLayout;
}

QHBoxLayout* EQAutoKeys::initPressInterval()
{
	auto* timeBetweenClickLayout{ new QHBoxLayout };

	auto* title{ new QLabel("Time between presses :") };
	timeBetweenClickLayout->addWidget(title);

	mPressIntervalEdit = new EQIntLineEdit(EQAutoKeysWorker::MIN_INTERVAL, EQAutoKeysWorker::MAX_INTERVAL, EQAutoKeysWorker::DEFAULT_BETWEEN_TIME);
	timeBetweenClickLayout->addWidget(mPressIntervalEdit);

	auto* unitsLabel{ new QLabel("ms") };
	timeBetweenClickLayout->addWidget(unitsLabel);

	connect(mPressIntervalEdit, &EQIntLineEdit::valueChanged, mAutoKeysWorker, &EQAutoKeysWorker::setPressInterval);

	return timeBetweenClickLayout;
}

void EQAutoKeys::disableWidgets()
{
	mTargetKeysPicker->setEnabled(false);
	mKeysHoldTimeEdit->setEnabled(false);
	mPressIntervalEdit->setEnabled(false);
	mShortcutPicker->setEnabled(false);
}

void EQAutoKeys::enableWidgets()
{
	mTargetKeysPicker->setEnabled(true);
	mKeysHoldTimeEdit->setEnabled(true);
	mPressIntervalEdit->setEnabled(true);
	mShortcutPicker->setEnabled(true);
}

void EQAutoKeys::switchState()
{
	mIsActive = !mIsActive;

	if (mIsActive)
	{
		mActivationStatusText->setText("Active");
		disableWidgets();
		mAutoKeysWorker->start();
	}
	else
	{
		mActivationStatusText->setText("Innactive");
		enableWidgets();
		mAutoKeysWorker->stop();
	}
}

void EQAutoKeys::setTargetKeys()
{
	mAutoKeysWorker->setTargetKeys(mTargetKeysPicker->targetKeys());
}

EQAutoKeys::~EQAutoKeys()
{
	mWorkerThread.quit();
	mWorkerThread.wait();
}