make:
	make build
	make run-server

build:
	g++ -o server src/main.cpp src/packetProcessor.cpp

run-server:
	./server

run-client:
	./client

clean:
	rm client
	rm server