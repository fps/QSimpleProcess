
.PHONY: all clean

all: test_qsimpleprocess

test_qsimpleprocess: test.cc QSimpleProcess.moc.o
	g++ -o test_qsimpleprocess test.cc QSimpleProcess.moc.o `pkg-config --libs QtCore`

qsimpleprocess.o: QSimpleProcess.h QSimpleProcess.moc.cc
	g++ -c QSimpleProcess.moc.cc `pkg-config --cflags QtCore`

QSimpleProcess.moc.cc: QSimpleProcess.h
	moc QSimpleProcess.h -o QSimpleProcess.moc.cc

clean:
	rm -f QSimpleProcess.moc.cc qsimpleprocess.o main test_qsimpleprocess