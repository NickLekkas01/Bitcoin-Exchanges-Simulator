compile: bitcoin.o functions.o
	gcc bitcoin.o functions.o -o bitcoin

bitcoin.o: bitcoin.c
	gcc bitcoin.c -c

functions.o: functions.c
	gcc functions.c -c

clean: functions.o bitcoin.o bitcoin
	rm functions.o bitcoin.o bitcoin
