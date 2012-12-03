#ifndef QSIMPLE_PROCESS_HH
#define QSIMPLE_PROCESS_HH

#include <QtCore/QProcess>
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QByteArray>
#include <QtCore/QCoreApplication>

#include <unistd.h>
#include <iostream>
#include <stdexcept>

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

	enum { IDLE, STARTING, RUNNING, FINISHED, PROCESS_ERROR, TIMEOUT_ERROR } state;
	
	public:
		QSimpleProcess(QObject *parent = 0) :
			QObject(parent),
			process(new QProcess(this)),
			timer(new QTimer(this)),
			state(IDLE)
		{
			
		}
		
	
	/**
	 * This static function tries to act somewhat like
	 * python's subprocess.check_output(). It constructs
	 * a QSimpleProcess instance, periodically calls 
	 * QApplication::processEvents() so that other event 
	 * processing can happen.
	 * 
	 * The stdout and stderr streams will hold the 
	 * output of the subprocess after run() returns.
	 * 
	 * This function will throw a runtime_error in
	 * case something went wrong.
	 */
	static void run(
		const QString program, 
		const QStringList arguments, 
		const QByteArray &stdin, 
		QByteArray &stdout, 
		QByteArray &stderr, 
		unsigned long timeout) {
		
		QSimpleProcess *process = new QSimpleProcess;
		
		process->start(program, arguments, stdin, timeout);
		
		std::cout << "processing" << std::endl;
		
		while ((process->state == IDLE) || (process->state == STARTING) || (process->state == RUNNING)) {
			// std::cout << "." << std::flush;
			usleep(10000);
			
			// std::cout << "," << std::flush;
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			// std::cout << ":" << std::flush;
		}
		
		// std::cout << "process not idle starting or running" << std::endl;
		
		if (process->state == TIMEOUT_ERROR) {
			throw std::runtime_error("Process timed out");
		}
		
		if (process->state == PROCESS_ERROR) {
			throw std::runtime_error("Processerror");
		}
		
		/**
		 * Only FINISHED left
		 */
		if (process->state != FINISHED) {
			throw std::logic_error("Application error - state not consistent");
		}
		
		stdout = process->stdout;
		stderr = process->stderr;
		
		delete process;
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
			state = STARTING;
			// std::cout << "start() end" << std::endl;
		}
	
	private slots:
		void process_error(QProcess::ProcessError process_error) {
			state = PROCESS_ERROR;
			// std::cout << "error" << std::endl;
			// process->close();
			emit error(process_error);
		}
		
		void process_finished(int exitCode, QProcess::ExitStatus exitStatus) {
			state = FINISHED;
			
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
			state = RUNNING;
			
			while(0 != stdin.length()) {
				stdin = stdin.mid(process->write(stdin));
			}
			
			process->closeWriteChannel();
		}
		
		void process_timeout() {
			//std::cout << "timeout" << std::endl;
			state = TIMEOUT_ERROR;
			// process->close();
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

