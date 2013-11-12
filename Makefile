train.o:
	g++ -c src/train.cpp -o obj/train.o

train: train.o
	g++ obj/train.o -o bin/train
	
clean:
	rm obj/*
	rm bin/*