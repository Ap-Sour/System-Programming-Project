OBJECTS = main.o functions.o 

minisearch: $(OBJECTS)
	gcc -g3 $(OBJECTS) -o minisearch

main.o: main.c functions.h
	gcc -c main.c 

functions.o: functions.c functions.h
	gcc -c functions.c

clean:
	rm *.o minisearch
