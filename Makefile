train.o:
	g++ -c src/model.cpp -o obj/model.o

GNB.o:
	g++ -c src/GNB.cpp -o obj/GNB.o
	
clean:
	rm obj/*
	rm bin/*