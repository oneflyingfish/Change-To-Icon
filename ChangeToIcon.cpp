#include "ChangeToIcon.h"
#include<qdebug.h>

ChangeToIcon::ChangeToIcon(QWidget *parent): QWidget(parent)
{
	GetForm();																			//初始化界面
	ConnectSignalAndSlot();																//绑定信号与槽+
}

void ChangeToIcon::ConnectSignalAndSlot()
{
	connect(hideWidgetButton, SIGNAL(clicked()), this, SLOT(ChangeExtend()));
	connect(aboutusButton, SIGNAL(clicked()), this, SLOT(AboutUs()));
	connect(exitButton, SIGNAL(clicked()), this, SLOT(ExitProgram()));

	connect(sizeRadiobutton_16, SIGNAL(clicked()), this, SLOT(ChooseSize16()));
	connect(sizeRadiobutton_32, SIGNAL(clicked()), this, SLOT(ChooseSize32()));
	connect(sizeRadiobutton_48, SIGNAL(clicked()), this, SLOT(ChooseSize48()));
	connect(sizeRadiobutton_64, SIGNAL(clicked()), this, SLOT(ChooseSize64()));
	connect(sizeRadiobutton_72, SIGNAL(clicked()), this, SLOT(ChooseSize72()));
	connect(sizeRadiobutton_96, SIGNAL(clicked()), this, SLOT(ChooseSize96()));
	connect(sizeRadiobutton_128, SIGNAL(clicked()), this, SLOT(ChooseSize128()));
}

void ChangeToIcon::ToIcon(QUrl url)
{
	QString oldFileName = url.fileName();
	QString oldFoldPath = url.adjusted(QUrl::RemoveFilename).toLocalFile();

	QString foldPath = "";
	//计算是Desktop还是源文件路径对应的路径
	if (this->extendWidget->savePathKind == SavePath::pivturePath)
	{
		foldPath = oldFoldPath;
	}
	else
	{
		foldPath= QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+"/";
	}

	//计算添加文件夹时路径
	QString fileName = foldPath;
	if (this->extendWidget->newFoldKind == NewFold::hasFold)
	{
		fileName += "Icon/";
		QDir *dir=new QDir();
		if (!dir->exists(fileName))
		{
			dir->mkdir(fileName);
		}
		delete dir;
	}

	//计算最终文件保存路径（含文件名）
	QString name = "";
	name+= oldFileName.section('.', 0, -2) + ".ico";
	fileName += name;

	qDebug() << fileName << endl;

	QPixmap pixmap(url.toLocalFile());

	if (this->extendWidget->autoCutChoose == AutoCut::yes)	//自动剪切图片
	{
		pixmap = AutoDealWithImage(pixmap).copy();
	}

	progressBar->setValue(progressBar->value() + n / 0.05);

	pixmap=pixmap.scaled(pictureSize, pictureSize);			//规定大小
	if (!pixmap.save(fileName, "ICO"))
	{
		if (pixmap.save(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + tr("/") + name, "ICO"))	//成功
		{
			QMessageBox::information(this,"提示",oldFileName + "写入路径错误，已经写入在桌面");
		}
		else
		{
			QMessageBox::information(this, "错误", oldFileName + "写入失败");
		}
	}
}

QPixmap ChangeToIcon::AutoDealWithImage(QPixmap pixmap)
{
	QImage image = pixmap.toImage();	//转化为image实现像素级处理
	image = image.convertToFormat(QImage::Format_ARGB32);	//支持透明色

	//估计背景色
	QColor color[8];
	int times[8];

	//颜色采样
	color[0] = image.pixelColor(2, 2);
	color[1] = image.pixelColor(image.width() / 2, 2);
	color[2] = image.pixelColor(image.width() - 3, 2);
	color[3] = image.pixelColor(2, image.height() / 2);
	color[4] = image.pixelColor(image.width() - 3, image.height() / 2);
	color[5] = image.pixelColor(2, image.height() - 3);
	color[6] = image.pixelColor(image.width() / 2, image.height() - 3);
	color[7] = image.pixelColor(image.width() - 3, image.height() - 3);

	for (int i = 0; i < 8; i++)
	{
		times[i] = 0;
	}

	for (int i = 0; i < 7; i++)
	{
		for (int j = i + 1; j < 8; j++)
		{
			if (color[i] == color[j])
			{
				times[i]++;
			}
		}
	}

	int index=0;
	int count=0;
	for(int i=0;i<8;i++)
	{
		if(times[i]==times[index])
		{
			count++;
		}
		else if(times[i]>times[index])
		{
			index=i;
			count=1;
		}
	}
	if(count<=1)
	{
		return pixmap;
	}
	
	QColor backgroundColor=color[index];
	
	progressBar->setValue(progressBar->value() + n * 0.3);

	//替换背景色
	for (int w = 0; w < image.width(); ++w)
	{
		for (int h = 0; h < image.height(); ++h)
		{
			QColor pixColor = image.pixelColor(w, h);
			if (pixColor == backgroundColor)
			{
				//替换颜色
				image.setPixelColor(w, h, QColor(0, 0, 0, 0));
			}
		}
	}
	progressBar->setValue(progressBar->value() + n * 0.3);


	//扩大图像
	int size = 0;
	if (image.height() >= image.width())
	{
		size = image.height();
	}
	else
	{
		size = image.width();
	}

	QPixmap pixmap0(size * 3, size * 3);
	pixmap0.fill(Qt::transparent);
	QPainter painter(&pixmap0);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.drawImage(QPoint(size, size), image);
	painter.end();
	image = pixmap0.toImage();	//kuo'zhong扩充后图片

	//自动剪裁图片
	int Ha1 = 0;									//平行两条线
	int Ha2 = image.height() - 1;
	int Va1 = 0;									//垂直两条线
	int Va2 = image.width() - 1;
	//扫描垂直线
	for (Va1 = 0; Va1 < image.width(); Va1++)
	{
		for (int h = 0; h < image.height(); h++)
		{
			if (image.pixelColor(Va1, h) != QColor(0, 0, 0, 0))
			{
				goto LOOP1;
			}
		}
	}
LOOP1:;

	for (Va2 = image.width() - 1; Va2 >= 0; Va2--)
	{
		for (int h = 0; h < image.height(); h++)
		{
			if (image.pixelColor(Va2, h) != QColor(0, 0, 0, 0))
			{
				goto LOOP2;
			}

		}
	}
LOOP2:;
	//扫描水平线
	for (Ha1 = 0; Ha1 < image.height(); Ha1++)
	{
		for (int h = 0; h < image.width(); h++)
		{
			if (image.pixelColor(h, Ha1) != QColor(0, 0, 0, 0))
			{
				goto LOOP3;
			}

		}
	}
LOOP3:;
	for (Ha2 = image.height() - 1; Ha1 >= 0; Ha2--)
	{
		for (int h = 0; h < image.width(); h++)
		{
			if (image.pixelColor(h, Ha2) != QColor(0, 0, 0, 0))
			{
				goto LOOP4;
			}

		}
	}
LOOP4:;

	int c;		//确保Ha1 < Ha2 ,  Va1<Va2
	if (Ha1 > Ha2)
	{
		c = Ha1;
		Ha1 = Ha2;
		Ha2 = c;
	}

	if (Va1 > Va2)
	{
		c = Va1;
		Va1 = Va2;
		Va2 = c;
	}
	progressBar->setValue(progressBar->value() + n * 0.3);

	int width = Va2 - Va1;
	int height = Ha2 - Ha1;

	int spaceSize = 2;
	if (width <= height)
	{
		return QPixmap::fromImage(image.copy(Va1 - (height - width) / 2 - spaceSize, Ha1 - spaceSize, height + spaceSize * 2, height + spaceSize * 2));
	}
	else
	{
		return QPixmap::fromImage(image.copy(Va1 - spaceSize, Ha1 - (width - height) / 2 - spaceSize, width + spaceSize * 2, width + spaceSize * 2));
	}
}

//重载事件分发器，实现QLabel接收拖拽
bool ChangeToIcon::eventFilter(QObject * obj, QEvent * ev)
{
	//dropLabel触发事件
	if (obj == this->dropLabel)
	{
		if (ev->type() == QEvent::DragEnter)	//触发文件拖入事件
		{
			QDragEnterEvent *eventEvent = dynamic_cast<QDragEnterEvent*>(ev);		//运行时类型识别（多态)
			if (count > 0)
			{
				eventEvent->ignore();
				return true;
			}

			if (eventEvent->mimeData()->urls().length() <= 0)
			{
				return true;
			}

			for (int i = 0; i < eventEvent->mimeData()->urls().length(); i++)
			{
				//存在一个文件不对
				if (eventEvent->mimeData()->urls()[i].toLocalFile().right(4).compare(".png", Qt::CaseInsensitive) && eventEvent->mimeData()->urls()[i].toLocalFile().right(4).compare(".jpg", Qt::CaseInsensitive) && eventEvent->mimeData()->urls()[i].toLocalFile().right(4).compare(".bmp", Qt::CaseInsensitive) && eventEvent->mimeData()->urls()[i].toLocalFile().right(5).compare(".jpeg", Qt::CaseInsensitive) && eventEvent->mimeData()->urls()[i].toLocalFile().right(4).compare(".ico", Qt::CaseInsensitive))
				{
					eventEvent->ignore();
					this->infoLabel->setText(tr("文件格式错误"));
					return true;
				}
			}
			eventEvent->acceptProposedAction();		//拖入文件均为图片
			return true;
		}
		else if (ev->type() == QEvent::Drop)			//文件放下事件
		{
			//调整状态栏提示
			this->infoLabel->setText(tr("请稍等..."));
			this->progressBar->setValue(0);
			this->progressBar->setVisible(true);

			//开始处理
			QTime time;								//测算运行时间
			time.restart();

			QDropEvent *eve = dynamic_cast<QDropEvent*>(ev);
			QList<QUrl> imageUrlList = eve->mimeData()->urls();
			count = imageUrlList.length();

			n = 100 / count;					//处理每张照片的完成度
			for (int i = 0; i < count; i++)
			{
				ToIcon(imageUrlList[i]);
				progressBar->setValue(n*(i + 1));		//处理完一张图片
			}

			progressBar->setValue(100);
			qDebug() << time.elapsed() << endl;
			this->infoLabel->setText(tr("已完成，耗时") + QString::number(time.elapsed(), 10) + tr("ms"));
			this->count = 0;

			return true;
		}
		else
		{
			return false;							//继续抛给dropLabel处理其它时间
		}
	}
	else
	{
		return eventFilter(obj, ev);				//继续上抛事件
	}
}

//定义界面初始化函数
void ChangeToIcon::GetForm()
{
	//定义主界面参数
	mainVBoxLayout->setSizeConstraint(QLayout::SetFixedSize);
	this->setWindowIcon(QIcon(":/ChangeToIcon/Icon/program.ico"));
	this->setWindowTitle(tr("Icon转化工具"));
	this->setMaximumWidth(310);
	this->setMaximumHeight(280);
	this->setStyleSheet("background-color: rgba(250, 250, 250,255);");

	//设置拖拽控件参数
	dropLabel->setStyleSheet("border:1px solid rgb(207, 207, 207);font: 9pt;color: gray;");
	dropLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	dropLabel->setFixedSize(150, 150);
	dropLabel->setAcceptDrops(true);													//设置接收拖拽
	dropLabel->installEventFilter(this);												//安装事件分发器

	//设置状态栏进度条信息不可见
	this->progressBar->setVisible(false);

	//设置大小按钮字体
	sizeRadiobutton_16->setStyleSheet("font: 10pt");
	sizeRadiobutton_32->setStyleSheet("font: 10pt");
	sizeRadiobutton_48->setStyleSheet("font: 10pt");
	sizeRadiobutton_64->setStyleSheet("font: 10pt");
	sizeRadiobutton_72->setStyleSheet("font: 10pt");
	sizeRadiobutton_96->setStyleSheet("font: 10pt");
	sizeRadiobutton_128->setStyleSheet("font: 10pt");

	//大小按钮布局
	radioVboxlayout->addWidget(sizeRadiobutton_16);
	radioVboxlayout->addWidget(sizeRadiobutton_32);
	radioVboxlayout->addWidget(sizeRadiobutton_48);
	radioVboxlayout->addWidget(sizeRadiobutton_64);
	radioVboxlayout->addWidget(sizeRadiobutton_72);
	radioVboxlayout->addWidget(sizeRadiobutton_96);
	radioVboxlayout->addWidget(sizeRadiobutton_128);
	radioVboxlayout->setMargin(0);
	radioVboxlayout->setSpacing(3);

	//设置大小选择互斥
	radioBox->addButton(sizeRadiobutton_16);
	radioBox->addButton(sizeRadiobutton_32);
	radioBox->addButton(sizeRadiobutton_48);
	radioBox->addButton(sizeRadiobutton_64);
	radioBox->addButton(sizeRadiobutton_72);
	radioBox->addButton(sizeRadiobutton_96);
	radioBox->addButton(sizeRadiobutton_128);

	//设置大小默认值
	sizeRadiobutton_128->setChecked(true);

	upHboxLayout->addWidget(dropLabel,1);
	upHboxLayout->addStretch();
	upHboxLayout->addLayout(radioVboxlayout,0);
	upHboxLayout->addStretch();
	upHboxLayout->setMargin(15);

	//定义功能按钮
	downHBoxLayout->addWidget(hideWidgetButton);
	downHBoxLayout->addStretch();
	downHBoxLayout->addWidget(aboutusButton);
	downHBoxLayout->addWidget(exitButton);
	downHBoxLayout->setMargin(5);

	//定义状态栏
	infoLabel->setStyleSheet("font: 9pt;color: red");
	infoLabel->setMaximumWidth(150);
	progressBar->setFixedSize(100,12);
	statusBar->addWidget(infoLabel);
	statusBar->addPermanentWidget(progressBar);
	statusBar->setFixedSize(287, 20);

	mainVBoxLayout->addLayout(upHboxLayout);
	mainVBoxLayout->addLayout(downHBoxLayout);
	mainVBoxLayout->addWidget(extendWidget);
	mainVBoxLayout->addWidget(statusBar);

	this->resize(310, 280);
	this->setLayout(mainVBoxLayout);
}

void ChangeToIcon::ChangeExtend()
{
	if (hideWidgetButton->text().endsWith(">>>"))
	{
		hideWidgetButton->setText(tr("隐藏设置<<<"));
		this->extendWidget->show();
	}
	else if(hideWidgetButton->text().endsWith("<<<"))
	{
		hideWidgetButton->setText(tr("展开设置>>>"));
		this->extendWidget->hide();
		this->resize(310, 280);
	}
	else
	{
		QMessageBox::information(this, tr("错误"), "位置拓展信息");
	}
}

void ChangeToIcon::AboutUs()
{
	QMessageBox::information(NULL, "提示", "本产品完全免费，抠图剪裁功能仅能支持纯色背景图片\n联系方式：a_flying_fish@outlook.com");
}

void ChangeToIcon::ExitProgram()
{
	this->close();
}

void ChangeToIcon::ChooseSize16()
{
	this->pictureSize = 16;
}

void ChangeToIcon::ChooseSize32()
{
	this->pictureSize = 32;
}

void ChangeToIcon::ChooseSize48()
{
	this->pictureSize = 48;
}

void ChangeToIcon::ChooseSize64()
{
	this->pictureSize = 64;
}

void ChangeToIcon::ChooseSize72()
{
	this->pictureSize = 72;
}

void ChangeToIcon::ChooseSize96()
{
	this->pictureSize = 96;
}

void ChangeToIcon::ChooseSize128()
{
	this->pictureSize = 128;
}

ChangeToIcon::~ChangeToIcon()
{
	dropLabel ->deleteLater();
	radioBox ->deleteLater();
	sizeRadiobutton_16 ->deleteLater();
	sizeRadiobutton_32 ->deleteLater();
	sizeRadiobutton_48 ->deleteLater();
	sizeRadiobutton_64 ->deleteLater();
	sizeRadiobutton_72 ->deleteLater();
	sizeRadiobutton_96 ->deleteLater();
	sizeRadiobutton_128 ->deleteLater();

	aboutusButton ->deleteLater();
	exitButton ->deleteLater();
	hideWidgetButton ->deleteLater();

	mainVBoxLayout ->deleteLater();
	upHboxLayout ->deleteLater();
	radioVboxlayout ->deleteLater();
	downHBoxLayout ->deleteLater();

	statusBar ->deleteLater();
	infoLabel ->deleteLater();
	progressBar ->deleteLater();

	extendWidget ->deleteLater();

	if (line != NULL)
	{
		delete line;
		line = NULL;
	}
}
