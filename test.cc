#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "QSimpleProcessTest.h"

#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	
	if (argc < 2) {
		std::cout << "Usage: test_qsimpleprocess cmd arg1 arg2 ..." << std::endl;
		return EXIT_FAILURE;
	}
	
	QSimpleProcessTest process;
	
	return app.exec();
}
