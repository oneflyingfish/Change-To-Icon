#include "extendWidget.h"

ExtendWidget::ExtendWidget(QWidget *parent): QWidget(parent)
{
	this->setStyleSheet("font: 14px");
	autoCutVboxLayout->addWidget(autoCutTrueButton);
	autoCutVboxLayout->addWidget(autoCutFalseButton);
	autoCutTrueButton->setToolTip(tr("有一定失败率，背景色纯度越高越可能成功"));
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

	ConnectSignalAndSlot();

	autoCutFalseButton->setChecked(true);
	picturePathButton->setChecked(true);
	noFoldButton->setChecked(true);

	autoCutChoose = AutoCut::no;
	savePathKind = SavePath::pivturePath;
	newFoldKind = NewFold::noFold;
}

ExtendWidget::~ExtendWidget()
{
	autoCutTrueButton->deleteLater();
	autoCutFalseButton ->deleteLater();

	picturePathButton ->deleteLater();
	desktopPathButton ->deleteLater();

	hasFoldButton ->deleteLater();
	noFoldButton ->deleteLater();

	autoCutGroupBox->deleteLater();
	savePathGroupBox->deleteLater();
	newFoldGroupBox->deleteLater();

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
