#!/bin/bash
echo "Compiling code."
make

FILE=./cachetime

if test -f "$FILE"; then 
	echo "Start measuring read latency using a single thread."
	python cachetime.py 6 20 cachetime
	python makeplots.py 1_thread_read.txt

	echo "Start measuring read/write latency using a single thread."
	python cachetime.py -m 6 20 cachetime 
	python makeplots.py 1_thread_write.txt
else
	echo "cachetime doesn't exist. Please check for code compiling."
fi

