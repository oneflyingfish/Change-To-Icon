#include "ChangeToIcon.h"
#include<qdebug.h>

ChangeToIcon::ChangeToIcon(QWidget *parent): QWidget(parent)
{
	GetForm();																			//��ʼ������
	ConnectSignalAndSlot();																//���ź����+
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
	//������Desktop����Դ�ļ�·����Ӧ��·��
	if (this->extendWidget->savePathKind == SavePath::pivturePath)
	{
		foldPath = oldFoldPath;
	}
	else
	{
		foldPath= QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+"/";
	}

	//��������ļ���ʱ·��
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

	//���������ļ�����·�������ļ�����
	QString name = "";
	name+= oldFileName.section('.', 0, -2) + ".ico";
	fileName += name;

	qDebug() << fileName << endl;

	QPixmap pixmap(url.toLocalFile());

	if (this->extendWidget->autoCutChoose == AutoCut::yes)	//�Զ�����ͼƬ
	{
		pixmap = AutoDealWithImage(pixmap).copy();
	}

	progressBar->setValue(progressBar->value() + n / 0.05);

	pixmap=pixmap.scaled(pictureSize, pictureSize);			//�涨��С
	if (!pixmap.save(fileName, "ICO"))
	{
		if (pixmap.save(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + tr("/") + name, "ICO"))	//�ɹ�
		{
			QMessageBox::information(this,"��ʾ",oldFileName + "д��·�������Ѿ�д��������");
		}
		else
		{
			QMessageBox::information(this, "����", oldFileName + "д��ʧ��");
		}
	}
}

QPixmap ChangeToIcon::AutoDealWithImage(QPixmap pixmap)
{
	QImage image = pixmap.toImage();	//ת��Ϊimageʵ�����ؼ�����
	image = image.convertToFormat(QImage::Format_ARGB32);	//֧��͸��ɫ

	//���Ʊ���ɫ
	QColor color[8];
	int times[8];

	//��ɫ����
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

	for (int i = 0; i < 8; i++)
	{
		if (times[i] == 1)
		{
			if (i == 7)
			{
				return pixmap;
			}
		}
		else
		{
			break;
		}

	}

	QColor backgroundColor;
	for (int i = 0; i < 8; i++)
	{
		if (i == 7)
		{
			backgroundColor = color[i];
			goto LOOP0;
		}

		for (int j = i + 1; j < 8; j++)
		{
			if (times[i] < times[j])
			{
				break;
			}

			if (j == 7)
			{
				backgroundColor = color[i];
				goto LOOP0;
			}
		}
	}

LOOP0:;
	progressBar->setValue(progressBar->value() + n * 0.3);

	//�滻����ɫ
	for (int w = 0; w < image.width(); ++w)
	{
		for (int h = 0; h < image.height(); ++h)
		{
			QColor pixColor = image.pixelColor(w, h);
			if (pixColor == backgroundColor)
			{
				//�滻��ɫ
				image.setPixelColor(w, h, QColor(0, 0, 0, 0));
			}
		}
	}
	progressBar->setValue(progressBar->value() + n * 0.3);


	//����ͼ��
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
	image = pixmap0.toImage();	//kuo'zhong�����ͼƬ

	//�Զ�����ͼƬ
	int Ha1 = 0;									//ƽ��������
	int Ha2 = image.height() - 1;
	int Va1 = 0;									//��ֱ������
	int Va2 = image.width() - 1;
	//ɨ�费ֱ��
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
	//ɨ��ˮƽ��
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

	int c;		//ȷ��Ha1 < Ha2 ,  Va1<Va2
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

//�����¼��ַ�����ʵ��QLabel������ק
bool ChangeToIcon::eventFilter(QObject * obj, QEvent * ev)
{
	//dropLabel�����¼�
	if (obj == this->dropLabel)
	{
		if (ev->type() == QEvent::DragEnter)	//�����ļ������¼�
		{
			QDragEnterEvent *eventEvent = dynamic_cast<QDragEnterEvent*>(ev);		//����ʱ����ʶ�𣨶�̬)
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
				//����һ���ļ�����
				if (eventEvent->mimeData()->urls()[i].toLocalFile().right(4).compare(".png", Qt::CaseInsensitive) && eventEvent->mimeData()->urls()[i].toLocalFile().right(4).compare(".jpg", Qt::CaseInsensitive) && eventEvent->mimeData()->urls()[i].toLocalFile().right(4).compare(".bmp", Qt::CaseInsensitive) && eventEvent->mimeData()->urls()[i].toLocalFile().right(5).compare(".jpeg", Qt::CaseInsensitive) && eventEvent->mimeData()->urls()[i].toLocalFile().right(4).compare(".ico", Qt::CaseInsensitive))
				{
					eventEvent->ignore();
					this->infoLabel->setText(tr("�ļ���ʽ����"));
					return true;
				}
			}
			eventEvent->acceptProposedAction();		//�����ļ���ΪͼƬ
			return true;
		}
		else if (ev->type() == QEvent::Drop)			//�ļ������¼�
		{
			//����״̬����ʾ
			this->infoLabel->setText(tr("���Ե�..."));
			this->progressBar->setValue(0);
			this->progressBar->setVisible(true);

			//��ʼ����
			QTime time;								//��������ʱ��
			time.restart();

			QDropEvent *eve = dynamic_cast<QDropEvent*>(ev);
			QList<QUrl> imageUrlList = eve->mimeData()->urls();
			count = imageUrlList.length();

			n = 100 / count;					//����ÿ����Ƭ����ɶ�
			for (int i = 0; i < count; i++)
			{
				ToIcon(imageUrlList[i]);
				progressBar->setValue(n*(i + 1));		//������һ��ͼƬ
			}

			progressBar->setValue(100);
			qDebug() << time.elapsed() << endl;
			this->infoLabel->setText(tr("����ɣ���ʱ") + QString::number(time.elapsed(), 10) + tr("ms"));
			this->count = 0;

			return true;
		}
		else
		{
			return false;							//�����׸�dropLabel��������ʱ��
		}
	}
	else
	{
		return eventFilter(obj, ev);				//���������¼�
	}
}

//��������ʼ������
void ChangeToIcon::GetForm()
{
	//�������������
	mainVBoxLayout->setSizeConstraint(QLayout::SetFixedSize);
	this->setWindowIcon(QIcon(":/ChangeToIcon/Icon/program.ico"));
	this->setWindowTitle(tr("Iconת������"));
	this->setMaximumWidth(310);
	this->setMaximumHeight(280);
	this->setStyleSheet("background-color: rgba(250, 250, 250,255);");

	//������ק�ؼ�����
	dropLabel->setStyleSheet("border:1px solid rgb(207, 207, 207);font: 9pt;color: gray;");
	dropLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	dropLabel->setFixedSize(150, 150);
	dropLabel->setAcceptDrops(true);													//���ý�����ק
	dropLabel->installEventFilter(this);												//��װ�¼��ַ���

	//����״̬����������Ϣ���ɼ�
	this->progressBar->setVisible(false);

	//���ô�С��ť����
	sizeRadiobutton_16->setStyleSheet("font: 10pt");
	sizeRadiobutton_32->setStyleSheet("font: 10pt");
	sizeRadiobutton_48->setStyleSheet("font: 10pt");
	sizeRadiobutton_64->setStyleSheet("font: 10pt");
	sizeRadiobutton_72->setStyleSheet("font: 10pt");
	sizeRadiobutton_96->setStyleSheet("font: 10pt");
	sizeRadiobutton_128->setStyleSheet("font: 10pt");

	//��С��ť����
	radioVboxlayout->addWidget(sizeRadiobutton_16);
	radioVboxlayout->addWidget(sizeRadiobutton_32);
	radioVboxlayout->addWidget(sizeRadiobutton_48);
	radioVboxlayout->addWidget(sizeRadiobutton_64);
	radioVboxlayout->addWidget(sizeRadiobutton_72);
	radioVboxlayout->addWidget(sizeRadiobutton_96);
	radioVboxlayout->addWidget(sizeRadiobutton_128);
	radioVboxlayout->setMargin(0);
	radioVboxlayout->setSpacing(3);

	//���ô�Сѡ�񻥳�
	radioBox->addButton(sizeRadiobutton_16);
	radioBox->addButton(sizeRadiobutton_32);
	radioBox->addButton(sizeRadiobutton_48);
	radioBox->addButton(sizeRadiobutton_64);
	radioBox->addButton(sizeRadiobutton_72);
	radioBox->addButton(sizeRadiobutton_96);
	radioBox->addButton(sizeRadiobutton_128);

	//���ô�СĬ��ֵ
	sizeRadiobutton_128->setChecked(true);

	upHboxLayout->addWidget(dropLabel,1);
	upHboxLayout->addStretch();
	upHboxLayout->addLayout(radioVboxlayout,0);
	upHboxLayout->addStretch();
	upHboxLayout->setMargin(15);

	//���幦�ܰ�ť
	downHBoxLayout->addWidget(hideWidgetButton);
	downHBoxLayout->addStretch();
	downHBoxLayout->addWidget(aboutusButton);
	downHBoxLayout->addWidget(exitButton);
	downHBoxLayout->setMargin(5);

	//����״̬��
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
		hideWidgetButton->setText(tr("��������<<<"));
		this->extendWidget->show();
	}
	else if(hideWidgetButton->text().endsWith("<<<"))
	{
		hideWidgetButton->setText(tr("չ������>>>"));
		this->extendWidget->hide();
		this->resize(310, 280);
	}
	else
	{
		QMessageBox::information(this, tr("����"), "λ����չ��Ϣ");
	}
}

void ChangeToIcon::AboutUs()
{
	QMessageBox::information(NULL, "��ʾ", "����Ʒ��ȫ��ѣ���ͼ���ù��ܽ���֧�ִ�ɫ����ͼƬ\n��ϵ��ʽ��a_flying_fish@outlook.com");
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