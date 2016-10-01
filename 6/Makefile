main: main.o GPIO.o
	g++ main.o GPIO.o -o main

main.o: main.cc GPIO.o
	g++ -c main.cc

GPIO.o: GPIO.cc GPIO.h
	g++ -c GPIO.cc

clean: 
	rm -f main.o GPIO.o main 
	
