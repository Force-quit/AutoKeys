#include "../Headers/EQAutoClicker.h"
#include "../Headers/EQAutoClickerWorker.h"
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

EQAutoClicker::EQAutoClicker()
{
	QWidget* centralWidget{ new QWidget };
	QVBoxLayout* centralLayout{ new QVBoxLayout };

	centralLayout->addWidget(initParameters());
	centralLayout->addWidget(initActivationLayout());

	mWidgetsToDisable.push_back(mClickHoldTimeEdit);
	mWidgetsToDisable.push_back(mClickIntervalEdit);
	mWidgetsToDisable.push_back(mLeftClickButton);
	mWidgetsToDisable.push_back(mRightClickButton);

	mShortcutPicker->startListening();
	centralWidget->setLayout(centralLayout);
	setCentralWidget(centralWidget);
	setWindowIcon(QIcon(":/images/mouse.png"));
}

QGroupBox* EQAutoClicker::initParameters()
{
	QGroupBox* parameters{ new QGroupBox("Parameters") };
	QVBoxLayout* parametersLayout{ new QVBoxLayout };
	parametersLayout->addLayout(initClickHoldTime());
	parametersLayout->addLayout(initClicksInterval());
	parametersLayout->addLayout(initClickButton());
	parameters->setLayout(parametersLayout);
	return parameters;
}

QGroupBox* EQAutoClicker::initActivationLayout()
{
	QGroupBox* activationGroupBox{ new QGroupBox("Activation") };
	QVBoxLayout* groupBoxLayout{ new QVBoxLayout };

	mShortcutPicker = new EQShortcutPicker("Activation shortcut :");

	QHBoxLayout* activationStatusLayout{ new QHBoxLayout };
	activationStatusLayout->setAlignment(Qt::AlignCenter);
	QLabel* activationStatusLabel{ new QLabel("Status :") };
	mActivationStatusText = new QLabel("Innactive");
	activationStatusLayout->addWidget(activationStatusLabel);
	activationStatusLayout->addWidget(mActivationStatusText);

	groupBoxLayout->addWidget(mShortcutPicker);
	groupBoxLayout->addLayout(activationStatusLayout);
	activationGroupBox->setLayout(groupBoxLayout);

	connect(mShortcutPicker, &EQShortcutPicker::startedChangingShortcut, this, &EQAutoClicker::disableWidgets);
	connect(mShortcutPicker, &EQShortcutPicker::stoppedChangingShortcut, this, &EQAutoClicker::enableWidgets);
	connect(mShortcutPicker, &EQShortcutPicker::shortcutPressed, mInputRecorderWorker, &EQAutoClickerWorker::switchState);

	connect(mInputRecorderWorker, &EQAutoClickerWorker::activated, this, &EQAutoClicker::disableWidgets);
	connect(mInputRecorderWorker, &EQAutoClickerWorker::deactivated, this, &EQAutoClicker::enableWidgets);

	connect(&mWorkerThread, &QThread::finished, mInputRecorderWorker, &QObject::deleteLater);
	mInputRecorderWorker->moveToThread(&mWorkerThread);
	mWorkerThread.start();

	return activationGroupBox;
}

QHBoxLayout* EQAutoClicker::initClickHoldTime()
{
	QHBoxLayout* clickHoldTimeLayout{ new QHBoxLayout };

	clickHoldTimeLayout->addWidget(new QLabel("Click hold time :"));

	mClickHoldTimeEdit = new EQIntLineEdit(EQAutoClickerWorker::MIN_INTERVAL, EQAutoClickerWorker::MAX_INTERVAL, EQAutoClickerWorker::DEFAULT_HOLD_TIME);
	clickHoldTimeLayout->addWidget(mClickHoldTimeEdit);

	clickHoldTimeLayout->addWidget(new QLabel("ms"));

	connect(mClickHoldTimeEdit, &EQIntLineEdit::valueChanged, mInputRecorderWorker, &EQAutoClickerWorker::setClickHoldTime);

	return clickHoldTimeLayout;
}

QHBoxLayout* EQAutoClicker::initClicksInterval()
{
	QHBoxLayout* timeBetweenClickLayout{ new QHBoxLayout };

	timeBetweenClickLayout->addWidget(new QLabel("Clicks interval :"));

	mClickIntervalEdit = new EQIntLineEdit(EQAutoClickerWorker::MIN_INTERVAL, EQAutoClickerWorker::MAX_INTERVAL, EQAutoClickerWorker::DEFAULT_BETWEEN_TIME);
	timeBetweenClickLayout->addWidget(mClickIntervalEdit);

	timeBetweenClickLayout->addWidget(new QLabel("ms"));

	connect(mClickIntervalEdit, &EQIntLineEdit::valueChanged, mInputRecorderWorker, &EQAutoClickerWorker::setClickInterval);

	return timeBetweenClickLayout;
}

QHBoxLayout* EQAutoClicker::initClickButton()
{
	QHBoxLayout* clickButtonLayout{ new QHBoxLayout };
	clickButtonLayout->setAlignment(Qt::AlignLeft);

	clickButtonLayout->addWidget(new QLabel("Mouse button"));

	QButtonGroup* clickButtonGroup{ new QButtonGroup };
	mLeftClickButton = new QRadioButton("Left");
	mLeftClickButton->click();
	mRightClickButton = new QRadioButton("Right");
	clickButtonGroup->addButton(mLeftClickButton);
	clickButtonGroup->addButton(mRightClickButton);

	clickButtonLayout->addWidget(mLeftClickButton);
	clickButtonLayout->addWidget(mRightClickButton);

	connect(clickButtonGroup, &QButtonGroup::buttonClicked, [this](QAbstractButton* pressedButton) {
		mInputRecorderWorker->setLeftClick(pressedButton == mLeftClickButton);
	});

	return clickButtonLayout;
}

void EQAutoClicker::disableWidgets()
{
	for (auto i : mWidgetsToDisable)
	{
		i->setDisabled(true);
	}
	if (mInputRecorderWorker->isActive())
	{
		mActivationStatusText->setText("Active");
	}
}

void EQAutoClicker::enableWidgets()
{
	for (auto i : mWidgetsToDisable)
	{
		i->setEnabled(true);
	}
	mActivationStatusText->setText("Innactive");
}

EQAutoClicker::~EQAutoClicker()
{
	mWorkerThread.quit();
	mWorkerThread.wait();
}