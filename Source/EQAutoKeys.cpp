#include "../Headers/EQAutoKeys.h"
#include "../Headers/EQAutoKeysWorker.h"
#include <EQUtilities/EQIntLineEdit.h>
#include <QBoxLayout>
#include <QCoreApplication>
#include <QGroupBox>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>

EQAutoKeys::EQAutoKeys()
{
	auto* centralWidget{ new QWidget };
	setCentralWidget(centralWidget);

	auto* centralLayout{ new QVBoxLayout };
	centralWidget->setLayout(centralLayout);
	centralLayout->addWidget(initParameters());
	centralLayout->addWidget(initActivationLayout());

	setWindowIcon(QIcon(":/images/mouse.png"));
}

QGroupBox* EQAutoKeys::initParameters()
{
	auto* wParameters{ new QGroupBox("Parameters") };

	auto* wParametersLayout{ new QVBoxLayout };
	wParameters->setLayout(wParametersLayout);

	wParametersLayout->addWidget(mTargetKeysPicker);
	mTargetKeysPicker->setTargetKeys(std::vector{ eutilities::UNKNOWN });

	wParametersLayout->addLayout(initKeysHoldTimeLayout());
	wParametersLayout->addLayout(initPressIntervalLayout());
	wParametersLayout->addLayout(initActivationDelayLayout());

	connect(mTargetKeysPicker, &EQShortcutPicker::startedChangingShortcut, this, &EQAutoKeys::disableWidgets);
	connect(mTargetKeysPicker, &EQShortcutPicker::startedChangingShortcut, mShortcutPicker, &EQShortcutPicker::disableButton);
	connect(mTargetKeysPicker, &EQShortcutPicker::startedChangingShortcut, mShortcutPicker, &EQShortcutPicker::stopListening);

	connect(mTargetKeysPicker, &EQShortcutPicker::stoppedChangingShortcut, this, &EQAutoKeys::enableWidgets);
	connect(mTargetKeysPicker, &EQShortcutPicker::stoppedChangingShortcut, mShortcutPicker, &EQShortcutPicker::enableButton);
	connect(mTargetKeysPicker, &EQShortcutPicker::stoppedChangingShortcut, this, &EQAutoKeys::setTargetKeys);
	connect(mTargetKeysPicker, &EQShortcutPicker::stoppedChangingShortcut, mShortcutPicker, &EQShortcutPicker::startListening);

	return wParameters;
}

QGroupBox* EQAutoKeys::initActivationLayout()
{
	auto* activationGroupBox{ new QGroupBox("Activation") };

	auto* groupBoxLayout{ new QVBoxLayout };
	activationGroupBox->setLayout(groupBoxLayout);

	groupBoxLayout->addWidget(mShortcutPicker);

	auto* activationStatusLayout{ new QHBoxLayout };
	groupBoxLayout->addLayout(activationStatusLayout);
	activationStatusLayout->setAlignment(Qt::AlignCenter);

	auto* activationStatusLabel{ new QLabel("Status :") };
	activationStatusLayout->addWidget(activationStatusLabel);

	mActivationStatusText = new QLabel("Innactive");
	activationStatusLayout->addWidget(mActivationStatusText);

	connect(mShortcutPicker, &EQShortcutPicker::startedChangingShortcut, this, &EQAutoKeys::disableWidgets);
	connect(mShortcutPicker, &EQShortcutPicker::startedChangingShortcut, mTargetKeysPicker, &EQShortcutPicker::disableButton);
	connect(mShortcutPicker, &EQShortcutPicker::stoppedChangingShortcut, this, &EQAutoKeys::enableWidgets);
	connect(mShortcutPicker, &EQShortcutPicker::stoppedChangingShortcut, mTargetKeysPicker, &EQShortcutPicker::enableButton);
	connect(mShortcutPicker, &EQShortcutPicker::shortcutPressed, this, &EQAutoKeys::switchState);

	connect(&mWorkerThread, &QThread::finished, mAutoKeysWorker, &QObject::deleteLater);
	mAutoKeysWorker->moveToThread(&mWorkerThread);
	mWorkerThread.start();

	return activationGroupBox;
}

QHBoxLayout* EQAutoKeys::initKeysHoldTimeLayout()
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

QHBoxLayout* EQAutoKeys::initPressIntervalLayout()
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

QHBoxLayout* EQAutoKeys::initActivationDelayLayout()
{
	auto* wActivationDelayLayout{ new QHBoxLayout };

	auto* title{ new QLabel("Activation delay :") };
	wActivationDelayLayout->addWidget(title);

	mActivationDelayEdit = new EQIntLineEdit(0, EQAutoKeysWorker::MAX_INTERVAL, EQAutoKeysWorker::DEFAULT_ACTIVATION_DELAY);
	wActivationDelayLayout->addWidget(mActivationDelayEdit);

	auto* unitsLabel{ new QLabel("ms") };
	wActivationDelayLayout->addWidget(unitsLabel);

	connect(mActivationDelayEdit, &EQIntLineEdit::valueChanged, mAutoKeysWorker, &EQAutoKeysWorker::setActivationDelay);

	return wActivationDelayLayout;
}

void EQAutoKeys::disableWidgets()
{
	mKeysHoldTimeEdit->setEnabled(false);
	mPressIntervalEdit->setEnabled(false);
	mActivationDelayEdit->setEnabled(false);
}

void EQAutoKeys::enableWidgets()
{
	mKeysHoldTimeEdit->setEnabled(true);
	mPressIntervalEdit->setEnabled(true);
	mActivationDelayEdit->setEnabled(true);
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