.PHONY: all clean

all: bin/mapper bin/reducer

bin/mapper:
	mkdir -p bin
	g++ -o bin/mapper_ave	mapper_ave.cpp
	g++ -o bin/mapper_dis mapper_dis.cpp

bin/reducer:
	mkdir -p bin
	g++ -o bin/reducer_ave reducer_ave.cpp
	g++ -o bin/reducer_dis reducer_dis.cpp

clean:
	rm -rf bin output
