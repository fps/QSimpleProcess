
.PHONY: all clean

all: test_qsimpleprocess test_qsimpleprocess2 qsimpleprocess-0.0.1.so

test_qsimpleprocess: test.cc QSimpleProcess.moc.o QSimpleProcessTest.moc.o
	g++ -o test_qsimpleprocess test.cc QSimpleProcess.moc.o QSimpleProcessTest.moc.o `pkg-config --cflags --libs QtCore`

test_qsimpleprocess2: test2.cc QSimpleProcess.moc.o
	g++ -o test_qsimpleprocess2 test2.cc QSimpleProcess.moc.o `pkg-config --cflags --libs QtCore`

	
qsimpleprocess-0.0.1.so:
	g++ -shared -fPIC -o qsimpleprocess-0.0.1.so QSimpleProcess.moc.o
	
QSimpleProcess.moc.o: QSimpleProcess.h QSimpleProcess.moc.cc
	g++ -fPIC -c QSimpleProcess.moc.cc `pkg-config --cflags QtCore`

QSimpleProcess.moc.cc: QSimpleProcess.h
	moc QSimpleProcess.h -o QSimpleProcess.moc.cc `pkg-config --cflags QtCore`

QSimpleProcessTest.moc.o: QSimpleProcessTest.h QSimpleProcessTest.moc.cc
	g++ -c QSimpleProcessTest.moc.cc `pkg-config --cflags QtCore`

QSimpleProcessTest.moc.cc: QSimpleProcessTest.h
	moc QSimpleProcessTest.h -o QSimpleProcessTest.moc.cc `pkg-config --cflags QtCore`

clean:
	rm -f QSimpleProcess.moc.cc QSimpleProcess.moc.o QSimpleProcessTest.moc.cc QSimpleProcessTest.moc.o qsimpleprocess.o main test_qsimpleprocess qsimpleprocess-0.0.1.so