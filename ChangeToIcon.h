#ifndef CHANGETOICON_H
#define CHANGETOICON_H

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#include<QtWidgets/QWidget>
#include<QLineEdit>
#include<QPushButton>
#include<QButtonGroup>
#include<QRadioButton>
#include<QStatusBar>
#include<QLabel>
#include<QMessageBox>
#include<QDropEvent>
#include<QDragEnterEvent>
#include<QSpacerItem> 
#include<QPixmap>
#include<QMimeData>
#include<QList>
#include<QStandardPaths>
#include<QUrl>
#include<QRgb>
#include<QFile>
#include<QPainter>
#include<QFileInfo>
#include<QDir>
#include<QTime>
#include<QProgressBar>
#include<QEvent>
#include<QBoxLayout>
#include"extendWidget.h"

class ChangeToIcon : public QWidget
{
	Q_OBJECT

public:
	ChangeToIcon(QWidget *parent = Q_NULLPTR);
	~ChangeToIcon();

	void GetForm();
	void ConnectSignalAndSlot();
	QPixmap AutoDealWithImage(QPixmap);

public slots:
	void ChooseSize16();
	void ChooseSize32();
	void ChooseSize48();
	void ChooseSize64();
	void ChooseSize72();
	void ChooseSize96();
	void ChooseSize128();
	void ChangeExtend();
	void AboutUs();
	void ExitProgram();
	void ToIcon(QUrl);

private:
	bool eventFilter(QObject *obj, QEvent *ev);

private:
	QLabel *dropLabel = new QLabel(tr("请拖入图片...\n支持png|jpg|jpeg|bmp|ico"));
	QButtonGroup *radioBox = new QButtonGroup();
	QRadioButton *sizeRadiobutton_16 = new QRadioButton("16*16");
	QRadioButton *sizeRadiobutton_32 = new QRadioButton("32*32");
	QRadioButton *sizeRadiobutton_48 = new QRadioButton("48*48");
	QRadioButton *sizeRadiobutton_64 = new QRadioButton("64*64");
	QRadioButton *sizeRadiobutton_72 = new QRadioButton("72*72");
	QRadioButton *sizeRadiobutton_96 = new QRadioButton("96*96");
	QRadioButton *sizeRadiobutton_128 = new QRadioButton("128*128");

	QPushButton *aboutusButton = new QPushButton(tr("关于"));
	QPushButton *exitButton = new QPushButton(tr("离开"));
	QPushButton *hideWidgetButton = new QPushButton(tr("展开设置>>>"));

	QVBoxLayout *mainVBoxLayout = new QVBoxLayout();
	QHBoxLayout *upHboxLayout = new QHBoxLayout();
	QVBoxLayout *radioVboxlayout = new QVBoxLayout();
	QHBoxLayout *downHBoxLayout = new QHBoxLayout();

	QStatusBar *statusBar = new QStatusBar();
	QLabel *infoLabel = new QLabel(tr("欢迎！"));
	QProgressBar *progressBar = new QProgressBar();

	QLine *line = new QLine();

	ExtendWidget *extendWidget = new ExtendWidget();

	int pictureSize = 128;
	int count = 0;
	int n = 0;						//完成一张图片进度条增加的数值
};

#endif