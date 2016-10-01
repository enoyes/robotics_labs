ServoPosition: ServoPosition.o GPIO.o 
	g++ ServoPosition.o GPIO.o -o ServoPosition

ServoPosition.o: ServoPosition.cc GPIO.o
	g++ -c ServoPosition.cc

GPIO.o: GPIO.cc GPIO.h
	g++ -c GPIO.cc

clean: 
	rm -f ServoPostion.o GPIO.o ServoPosition
	
