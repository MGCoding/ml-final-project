model.o:
	g++ -c src/model.cpp -o obj/model.o -O3

GNB.o:
	g++ -c src/GNB.cpp -o obj/GNB.o -O3
	
DT.o:
	g++ -c src/DT.cpp -o obj/DT.o -O3

kNN.o:
	g++ -c src/kNN.cpp -o obj/kNN.o -O3
	
GNB-test: GNB.o model.o
	g++ obj/GNB.o obj/model.o src/GNB-test.cpp -o bin/GNB-test -O3
	
DT-test: DT.o model.o
	g++ obj/DT.o obj/model.o src/DT-test.cpp -o bin/DT-test -O3
	
kNN-test: kNN.o model.o
	g++ obj/kNN.o obj/model.o src/kNN-test.cpp -o bin/kNN-test -O3
	
clean:
	rm obj/*
	rm bin/*