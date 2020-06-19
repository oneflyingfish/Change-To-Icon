#include "extendWidget.h"

ExtendWidget::ExtendWidget(QWidget *parent): QWidget(parent)
{
	this->setStyleSheet("font: 14px");
	autoCutVboxLayout->addWidget(autoCutTrueButton);
	autoCutVboxLayout->addWidget(autoCutFalseButton);
	autoCutTrueButton->setToolTip(tr("基于GrabCut算法，比较耗时"));
	autoCutHBoxLayout->addWidget(iterationsLabel);
	autoCutHBoxLayout->addWidget(iterationsSpinBox);
	autoCutHBoxLayout->addStretch();
	autoCutVboxLayout->addLayout(autoCutHBoxLayout);
	autoCutVboxLayout->setSpacing(3);
	autoCutGroupBox->setLayout(autoCutVboxLayout);

	savePathVboxLayout->addWidget(desktopPathButton);
	savePathVboxLayout->addWidget(picturePathButton);
	savePathVboxLayout->setSpacing(3);
	savePathGroupBox->setLayout(savePathVboxLayout);

	newFoldVboxLayout->addWidget(hasFoldButton);
	newFoldVboxLayout->addWidget(noFoldButton);
	newFoldVboxLayout->setSpacing(3);
	newFoldGroupBox->setLayout(newFoldVboxLayout);

	vbox->addWidget(autoCutGroupBox);
	vbox->addWidget(savePathGroupBox);
	vbox->addWidget(newFoldGroupBox);
	vbox->setSpacing(10);
	this->setLayout(vbox);
	this->hide();
	this->frameGeometry().setWidth(310);

	this->ConnectSignalAndSlot();

	this->autoCutFalseButton->setChecked(true);
	this->picturePathButton->setChecked(true);
	this->noFoldButton->setChecked(true);

	this->autoCutChoose = AutoCut::no;
	this->savePathKind = SavePath::pivturePath;
	this->newFoldKind = NewFold::noFold;

	this->iterationsSpinBox->setMaximum(30);
	this->iterationsSpinBox->setMinimum(1);
	this->iterationsSpinBox->setValue(10);
	this->iterationsLabel->setToolTip(tr("迭代次数越大越耗时，但效果相对会更好"));
}

int ExtendWidget::getIterations()
{
	return this->iterationsSpinBox->value();
}

ExtendWidget::~ExtendWidget()
{
	autoCutTrueButton->deleteLater();
	autoCutFalseButton ->deleteLater();
	iterationsLabel->deleteLater();
	iterationsSpinBox->deleteLater();

	picturePathButton ->deleteLater();
	desktopPathButton ->deleteLater();

	hasFoldButton ->deleteLater();
	noFoldButton ->deleteLater();

	autoCutGroupBox->deleteLater();
	savePathGroupBox->deleteLater();
	newFoldGroupBox->deleteLater();

	autoCutHBoxLayout->deleteLater();
	savePathVboxLayout->deleteLater();
	autoCutVboxLayout->deleteLater();
	newFoldVboxLayout->deleteLater();

	vbox ->deleteLater();
}

void ExtendWidget::ConnectSignalAndSlot()
{
	connect(autoCutTrueButton, SIGNAL(clicked()), this, SLOT(autoCutTrueCheck()));
	connect(autoCutFalseButton, SIGNAL(clicked()), this, SLOT(autoCutFalseCheck()));
	connect(desktopPathButton, SIGNAL(clicked()), this, SLOT(desktopPathCheck()));
	connect(picturePathButton, SIGNAL(clicked()), this, SLOT(picturePathCheck()));
	connect(hasFoldButton, SIGNAL(clicked()), this, SLOT(hasFoldCheck()));
	connect(noFoldButton, SIGNAL(clicked()), this, SLOT(noFoldCheck()));
}

void ExtendWidget::autoCutTrueCheck()
{
	this->autoCutChoose = AutoCut::yes;
}

void ExtendWidget::autoCutFalseCheck()
{
	this->autoCutChoose = AutoCut::no;
}

void ExtendWidget::picturePathCheck()
{
	this->savePathKind = SavePath::pivturePath;
}

void ExtendWidget::desktopPathCheck()
{
	this->savePathKind = SavePath::desktop;
}

void ExtendWidget::hasFoldCheck()
{
	this->newFoldKind = NewFold::hasFold;
}

void ExtendWidget::noFoldCheck()
{
	this->newFoldKind = NewFold::noFold;
}
