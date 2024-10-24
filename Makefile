make:
	make build
	make run-server

build:
	g++ -o server src/UDPServer.cpp
	g++ -o client src/UDPClient.cpp

run-server:
	./server

run-client:
	./client

clean:
	rm client
	rm server