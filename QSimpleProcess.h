#ifndef QSIMPLE_PROCESS_HH
#define QSIMPLE_PROCESS_HH

#include <QtCore/QProcess>
#include <QtCore/QObject>

class QSimpleProcess : public QObject {
	Q_OBJECT
	
	QProcess *process;
	
	QSimpleProcess(QObject *parent = 0) :
		QObject(parent),
		process(new QProcess(this))
	{
		
	}
	
	public slots:
		void call(
			QString program,
			QStringList arguments
		) {
			
			
		}
		
	signals:
		void finished(QString stdout, QString stderr);
};

#endif

