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

QGroupBox* EQAutoKeys::initParameters()
{
	QGroupBox* parameters{ new QGroupBox("Parameters") };
	QVBoxLayout* parametersLayout{ new QVBoxLayout };
	parametersLayout->addLayout(initClickHoldTime());
	parametersLayout->addLayout(initClicksInterval());
	parametersLayout->addLayout(initClickButton());
	parameters->setLayout(parametersLayout);
	return parameters;
}

QGroupBox* EQAutoKeys::initActivationLayout()
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

	connect(mShortcutPicker, &EQShortcutPicker::startedChangingShortcut, this, &EQAutoKeys::disableWidgets);
	connect(mShortcutPicker, &EQShortcutPicker::stoppedChangingShortcut, this, &EQAutoKeys::enableWidgets);
	connect(mShortcutPicker, &EQShortcutPicker::shortcutPressed, mInputRecorderWorker, &EQAutoKeysWorker::switchState);

	connect(mInputRecorderWorker, &EQAutoKeysWorker::activated, this, &EQAutoKeys::disableWidgets);
	connect(mInputRecorderWorker, &EQAutoKeysWorker::deactivated, this, &EQAutoKeys::enableWidgets);

	connect(&mWorkerThread, &QThread::finished, mInputRecorderWorker, &QObject::deleteLater);
	mInputRecorderWorker->moveToThread(&mWorkerThread);
	mWorkerThread.start();

	return activationGroupBox;
}

QHBoxLayout* EQAutoKeys::initClickHoldTime()
{
	QHBoxLayout* clickHoldTimeLayout{ new QHBoxLayout };

	clickHoldTimeLayout->addWidget(new QLabel("Click hold time :"));

	mClickHoldTimeEdit = new EQIntLineEdit(EQAutoKeysWorker::MIN_INTERVAL, EQAutoKeysWorker::MAX_INTERVAL, EQAutoKeysWorker::DEFAULT_HOLD_TIME);
	clickHoldTimeLayout->addWidget(mClickHoldTimeEdit);

	clickHoldTimeLayout->addWidget(new QLabel("ms"));

	connect(mClickHoldTimeEdit, &EQIntLineEdit::valueChanged, mInputRecorderWorker, &EQAutoKeysWorker::setClickHoldTime);

	return clickHoldTimeLayout;
}

QHBoxLayout* EQAutoKeys::initClicksInterval()
{
	QHBoxLayout* timeBetweenClickLayout{ new QHBoxLayout };

	timeBetweenClickLayout->addWidget(new QLabel("Clicks interval :"));

	mClickIntervalEdit = new EQIntLineEdit(EQAutoKeysWorker::MIN_INTERVAL, EQAutoKeysWorker::MAX_INTERVAL, EQAutoKeysWorker::DEFAULT_BETWEEN_TIME);
	timeBetweenClickLayout->addWidget(mClickIntervalEdit);

	timeBetweenClickLayout->addWidget(new QLabel("ms"));

	connect(mClickIntervalEdit, &EQIntLineEdit::valueChanged, mInputRecorderWorker, &EQAutoKeysWorker::setClickInterval);

	return timeBetweenClickLayout;
}

QHBoxLayout* EQAutoKeys::initClickButton()
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

void EQAutoKeys::disableWidgets()
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

void EQAutoKeys::enableWidgets()
{
	for (auto i : mWidgetsToDisable)
	{
		i->setEnabled(true);
	}
	mActivationStatusText->setText("Innactive");
}

EQAutoKeys::~EQAutoKeys()
{
	mWorkerThread.quit();
	mWorkerThread.wait();
}