model.o:
	g++ -c src/model.cpp -o obj/model.o -O3

GNB.o:
	g++ -c src/GNB.cpp -o obj/GNB.o -O3
	
DT.o:
	g++ -c src/DT.cpp -o obj/DT.o -O3

kNN.o:
	g++ -c src/kNN.cpp -o obj/kNN.o -O3

MCP.o:
	g++ -c src/MCP.cpp -o obj/MCP.o -O3

MLP.o:
	g++ -c src/MLP.cpp -o obj/MLP.o -O3

GNB-test: GNB.o model.o
	g++ obj/GNB.o obj/model.o src/GNB-test.cpp -o bin/GNB-test -O3
	
DT-test: DT.o model.o
	g++ obj/DT.o obj/model.o src/DT-test.cpp -o bin/DT-test -O3
	
kNN-test: kNN.o model.o
	g++ obj/kNN.o obj/model.o src/kNN-test.cpp -o bin/kNN-test -O3

MCP-test: MCP.o model.o
	g++ obj/MCP.o obj/model.o src/MCP-test.cpp -o bin/MCP-test -O3

MLP-test: MLP.o model.o
	g++ obj/MLP.o obj/model.o src/MLP-test.cpp -o bin/MLP-test -O3

	
clean:
	rm obj/*
	rm bin/*