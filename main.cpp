#include "ChangeToIcon.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ChangeToIcon w;
	w.show();

	/*ExtendWidget e;
	e.show();*/
	return a.exec();
}
