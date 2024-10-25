#!/bin/bash

while :
do
	inotifywait -e modify ./src
	killall server
	make build
	make run-server &
done
