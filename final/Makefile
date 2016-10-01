main: main.o RoboticWiimote.o
	g++ main.o RoboticWiimote.o -o main

main.o: main.cc RoboticWiimote.h
	g++ -c main.cc

RoboticWiimote.o: RoboticWiimote.cc RoboticWiimote.h
	g++ -c RoboticWiimote.cc

clean:
	rm -f main.o RoboticWiimote.o
