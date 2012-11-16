
.PHONY: all clean

all: test_qsimpleprocess

test_qsimpleprocess: test.cc QSimpleProcess.moc.o QSimpleProcessTest.moc.o
	g++ -o test_qsimpleprocess test.cc QSimpleProcess.moc.o QSimpleProcessTest.moc.o `pkg-config --cflags --libs QtCore`

QSimpleProcess.moc.o: QSimpleProcess.h QSimpleProcess.moc.cc
	g++ -c QSimpleProcess.moc.cc `pkg-config --cflags QtCore`

QSimpleProcess.moc.cc: QSimpleProcess.h
	moc QSimpleProcess.h -o QSimpleProcess.moc.cc `pkg-config --cflags QtCore`

QSimpleProcessTest.moc.o: QSimpleProcessTest.h QSimpleProcessTest.moc.cc
	g++ -c QSimpleProcessTest.moc.cc `pkg-config --cflags QtCore`

QSimpleProcessTest.moc.cc: QSimpleProcessTest.h
	moc QSimpleProcessTest.h -o QSimpleProcessTest.moc.cc `pkg-config --cflags QtCore`

clean:
	rm -f QSimpleProcess.moc.cc qsimpleprocess.o main test_qsimpleprocess