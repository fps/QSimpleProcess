#include "QSimpleProcess.h"

#include <iostream>

#include <QtCore/QCoreApplication>

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	
	QString program("cat");
	QStringList args;
	
	QByteArray stdin("Bla");
	QByteArray stdout;
	QByteArray stderr;
	
	try {
		QSimpleProcess::run(program, args, stdin, stdout, stderr, 10000);
	} catch (std::runtime_error e) {
		std::cout << "oops: " << e.what() << std::endl;
	}
	
	std::cout << QString(stdout).toStdString() << std::endl;
	std::cout << QString(stderr).toStdString() << std::endl;
}