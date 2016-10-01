#include "Wiimote.h"
#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

Wiimote::Wiimote()
{
	fd = open("/dev/input/event0", O_RDONLY);
	if (fd == -1)
 	{
 		std::cerr << "Error: Could not open event file - forgot sudo?\n";
 		exit(1);
 	}
}

Wiimote::~Wiimote()
{
	close(fd);
}

void Wiimote::Listen()
{
	while(true)
	{
		// Read a packet of 32 bytes from Wiimote
 		char buffer[32];
 		read(fd, buffer, 32);

 		// Extract code (byte 10) and value (byte 12) from packet
 		int code = buffer[10];
 		short acceleration = * (short *) (buffer + 12);

		if (code)
			AccelerationEvent(code, acceleration);
	}
}

void Wiimote::AccelerationEvent(int code, int acceleration)
{
	// Print code and value
	 std::cout << "Code = " << code <<
			", acceleration = " << acceleration << '\n';
}
