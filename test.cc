#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "QSimpleProcessTest.h"

#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	QSimpleProcessTest process("cat", QStringList(), "hello world", -1);

	return app.exec();
}
