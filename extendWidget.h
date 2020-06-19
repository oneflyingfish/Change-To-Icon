#ifndef EXTENDWIDGET_H
#define EXTENDWIDGET_H

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#include<QtWidgets/QWidget>
#include<QRadioButton>
#include<QButtonGroup>
#include<QGroupBox>
#include<QBoxLayout>
#include<QSpinBox>
#include<QHBoxLayout>
#include<QLabel>
#include"enumChoose.h"


class ExtendWidget: public QWidget
{
	Q_OBJECT
public:
	ExtendWidget(QWidget *parent = Q_NULLPTR);
	~ExtendWidget();
	void ConnectSignalAndSlot();

public slots:
	void autoCutTrueCheck();
	void autoCutFalseCheck();
	void picturePathCheck();
	void desktopPathCheck();
	void hasFoldCheck();
	void noFoldCheck();

	int getIterations();

public:
	AutoCut autoCutChoose;
	SavePath savePathKind;
	NewFold newFoldKind;


private:
	QGroupBox *autoCutGroupBox = new QGroupBox(tr("自动背景透明并剪裁"));
	QGroupBox *savePathGroupBox = new QGroupBox(tr("保存路径选项"));
	QGroupBox *newFoldGroupBox = new QGroupBox(tr("文件夹选项"));

	QVBoxLayout *autoCutVboxLayout = new QVBoxLayout();
	QHBoxLayout *autoCutHBoxLayout = new QHBoxLayout();
	QVBoxLayout *savePathVboxLayout = new QVBoxLayout();
	QVBoxLayout *newFoldVboxLayout = new QVBoxLayout();

	QRadioButton *autoCutTrueButton = new QRadioButton(tr("是"));
	QRadioButton *autoCutFalseButton = new QRadioButton(tr("否"));
	QLabel* iterationsLabel = new QLabel(tr("迭代次数: "));
	QSpinBox* iterationsSpinBox = new QSpinBox();

	QRadioButton *picturePathButton = new QRadioButton(tr("源文件路径"));
	QRadioButton *desktopPathButton = new QRadioButton(tr("桌面"));

	QRadioButton *hasFoldButton = new QRadioButton(tr("放在Icon文件夹中"));
	QRadioButton *noFoldButton = new QRadioButton(tr("不做处理"));

	QVBoxLayout *vbox = new QVBoxLayout();
};

#endif
