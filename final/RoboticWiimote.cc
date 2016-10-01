#include "RoboticWiimote.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

RoboticWiimote::RoboticWiimote(){

  w_fd = open("/dev/input/event0", O_RDONLY);
	if (w_fd == -1)
 	{
 		std::cerr << "Error: Could not open event file - forgot sudo?\n";
 		exit(1);
 	}

  // Open memory mapped I/O
  r_fd = open("/dev/mem", O_RDWR);

  // Map physical memory
  ptr = (char *) mmap(NULL, 0xff, PROT_READ | PROT_WRITE, MAP_SHARED, r_fd, 0x400d0000);

  // Check success
  if (ptr == (void *) -1)
  {
    std::cerr << "Error mapping memory - fogot sudo?\n";
    exit(1);
  }

}

RoboticWiimote::~RoboticWiimote(){
  // Unmap physical memory
  munmap(ptr, 0xff);
  // Close memory mapped I/O
  close(r_fd);
  close(w_fd);

}

void RoboticWiimote::Listen()
{
	while(true)
	{
		// Read a packet of 32 bytes from Wiimote
 		char buffer[32];
 		read(w_fd, buffer, 32);

 		// Extract code (byte 10) and value (byte 12) from packet
 		int code = buffer[10];
 		short acceleration = * (short *) (buffer + 12);

		if (code == 4)
			AccelerationEvent(code, acceleration);
	}
}

void RoboticWiimote::AccelerationEvent(int code, int acceleration)
{
  int angle = acceleration + 15;
  angle = angle * 9;
  angle = angle / -10;

  if (code == 4){

    std::cout << "Code = " << code <<
    ", angle = " << angle << '\n';

    MoveServo(4, angle, 35);
  }

}

void RoboticWiimote::RegisterWrite(unsigned offset, unsigned value)
{
 	* (volatile unsigned *) (ptr + offset) = value;
}

void RoboticWiimote::MoveServo(int id, int angle, int speed)
{
 	// Check valid servo
 	if (id < 0 || id > 4)
 	{
 		std::cerr << "Invalid servo ID\n";
 		exit(1);
 	}

 	// Verify valid angle
 	if (angle < 0 || angle > 180)
 	{
 		std::cerr << "Invalid angle\n";
 		exit(1);
 	}

 	// Set memory locations
 	RegisterWrite(0x100 + id * 8, angle);
 	RegisterWrite(0x100 + id * 8 + 4, speed);
}
