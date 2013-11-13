<<<<<<< HEAD
model.o:
=======
train.o:
>>>>>>> 5fdba5320df8d921cc0f262662cf6cfff5d0e8f5
	g++ -c src/model.cpp -o obj/model.o

GNB.o:
	g++ -c src/GNB.cpp -o obj/GNB.o
	
GNB-test: GNB.o model.o
	g++ obj/GNB.o obj/model.o src/GNB-test.cpp -o bin/GNB-test -O3
clean:
	rm obj/*
	rm bin/*