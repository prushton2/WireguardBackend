#!/bin/bash

while :
do
	inotifywait -e modify ./src
	killall server
	tput reset
	make build
	make run-server &
done
