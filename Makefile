
make: cachetime.c
	gcc cachetime.c -lm -lpthread -o cachetime
clean: 
	rm -f cachetime
