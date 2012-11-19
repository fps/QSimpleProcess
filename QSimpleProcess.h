#ifndef QSIMPLE_PROCESS_HH
#define QSIMPLE_PROCESS_HH

#include <QtCore/QProcess>
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QByteArray>

#include <iostream>
/**
 * A simple wrapper around a QProcess that tries to
 * reliably encapsulate an often needed usecase:
 * 
 * Using a process similar to a function call:
 * 
 * - Feed in some data through arguments and stdin
 * - Close stdin
 * - Fetch all resulting output on stdout and stderr
 *   and the exit code
 */
class QSimpleProcess : public QObject {
	Q_OBJECT
	
	QProcess *process;
	QTimer *timer;
	
	QByteArray stdout;
	QByteArray stderr;
	QByteArray stdin;

	public:
		QSimpleProcess(QObject *parent = 0) :
			QObject(parent),
			process(new QProcess(this)),
			timer(new QTimer(this))
		{
			
		}
	
	public slots:
		/**
		 * Do not call this slot again before either
		 * finished() or error() signals were emitted
		 */
		void start(
			QString program,
			QStringList arguments,
			QByteArray stdin,
			unsigned long timeout
		) {
			stdout = "";
			stderr = "";
			this->stdin = stdin;
			
			connect(process, SIGNAL(started()), this, SLOT(process_started()));
			connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(process_ready_read_stdout()));
			connect(process, SIGNAL(readyReadStandardError()), this, SLOT(process_ready_read_stderr()));
			connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(process_finished(int, QProcess::ExitStatus)));
			connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(process_error(QProcess::ProcessError)));
			
			timer->setSingleShot(true);
			timer->start(timeout);
			connect(timer, SIGNAL(timeout()), this, SLOT(process_timeout()));
			
			process->start(program, arguments);
		}
	
	private slots:
		void process_error(QProcess::ProcessError process_error) {
			// std::cout << "error" << std::endl;
			process->close();
			emit error(process_error);
		}
		
		void process_finished(int exitCode, QProcess::ExitStatus exitStatus) {
			stdout.append(process->readAllStandardOutput());
			stderr.append(process->readAllStandardError());
			emit finished(exitCode, stdout, stderr);
		}
		
		void process_ready_read_stdout() {
			stdout.append(process->readAllStandardOutput());
		}

		void process_ready_read_stderr() {
			stderr.append(process->readAllStandardError());
		}

		void process_started() {
			while(0 != stdin.length()) {
				stdin = stdin.mid(process->write(stdin));
			}
			
			process->closeWriteChannel();
		}
		
		void process_timeout() {
			process->close();
			emit error(QProcess::Timedout);
		}

	signals:
		/**
		 * If the error signal was emitted, then the
		 * finished signal will never be called and the
		 * process will be killed (if it isn't dead yet).
		 * 
		 * Only finished() or error() will be emitted
		 * and exactly one of them will be emitted given
		 * that the process actually finishes at some time
		 * or the timeout has fired.
		 */
		void error(QProcess::ProcessError error);
		
		/**
		 * finished will be emitted when the process has
		 * finished without error and all stdout and stderr
		 * output has been successfully read
		 * 
		 * Only finished() or error() will be emitted
		 * and exactly one of them will be emitted given
		 * that the process actually finishes at some time
		 * or the timeout has fired.
		 */
		void finished(int exitCode, QByteArray stdout, QByteArray stderr);
};

#endif

