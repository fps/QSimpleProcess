#ifndef QSIMPLE_PROCESS_TEST_HH
#define QSIMPLE_PROCESS_TEST_HH

#include <QtCore/QObject>
#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "QSimpleProcess.h"

#include <iostream>

class QSimpleProcessTest : public QObject {
	Q_OBJECT

	QSimpleProcess process;

	public:
		QSimpleProcessTest() {
			connect(&process, SIGNAL(error (QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
			connect(&process, SIGNAL(finished(int, QByteArray, QByteArray)), this, SLOT(finished(int, QByteArray, QByteArray)));
			
			std::cout << "starting" << std::endl;
			process.start("echo", QStringList() << "bla" << "bla", "Bla Bla Bla", -1);
		}
	
	public slots:
		void finished(int exitStatus, QByteArray stdout, QByteArray stderr) {
			std::cout << "finished: " << std::endl;
			std::cout << stdout.data() << std::endl;
			std::cout << stderr.data() << std::endl;
			
			QCoreApplication::exit(0);
		}
		
		void error(QProcess::ProcessError process_error) {
			std::cout << "error" << process_error << std::endl;
			QCoreApplication::exit(1);
		}
};

#endif

