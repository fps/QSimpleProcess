#include "QSimpleProcess.h"

#include <iostream>

#include <QtCore/QCoreApplication>

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	
	try {
		std::cout << "test1" << std::endl;
		
		QByteArray stdin("Hello World!");
		QByteArray stdout;
		QByteArray stderr;
	
		QString program("cat");
		QStringList args;
		
		QSimpleProcess::run(program, args, stdin, stdout, stderr, 1000);
		
		std::cout << "stdout: " << QString(stdout).toStdString() << std::endl;
		std::cout << "stderr: " << QString(stderr).toStdString() << std::endl;
		
	} catch (std::runtime_error e) {
		std::cout << "oops: " << e.what() << std::endl;
	}

	try {
		std::cout << "test2" << std::endl;
		
		QByteArray stdin;
		QByteArray stdout;
		QByteArray stderr;
	
		QString program("sleep");
		QStringList args;
		args << "2";
		
		QSimpleProcess::run(program, args, stdin, stdout, stderr, 100);
		
		std::cout << "stdout: " << QString(stdout).toStdString() << std::endl;
		std::cout << "stderr: " << QString(stderr).toStdString() << std::endl;
		
	} catch (std::runtime_error e) {
		std::cout << "oops: " << e.what() << std::endl;
	}

}