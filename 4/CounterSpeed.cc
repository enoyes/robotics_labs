#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <iostream>
#include <time.h>

// Physical base address of GPIO
const unsigned gpio_address = 0x400d0000;

// Length of memory-mapped IO window
const unsigned gpio_size = 0xff;

const int gpio_led1_offset = 0x12C; // Offset for LED1
const int gpio_led2_offset = 0x130; // Offset for LED2
const int gpio_led3_offset = 0x134; // Offset for LED3
const int gpio_led4_offset = 0x138; // Offset for LED4
const int gpio_led5_offset = 0x13C; // Offset for LED5
const int gpio_led6_offset = 0x140; // Offset for LED6
const int gpio_led7_offset = 0x144; // Offset for LED7
const int gpio_led8_offset = 0x148; // Offset for LED8

const int gpio_sw1_offset = 0x14C; // Offset for Switch 1
const int gpio_sw2_offset = 0x150; // Offset for Switch 2
const int gpio_sw3_offset = 0x154; // Offset for Switch 3
const int gpio_sw4_offset = 0x158; // Offset for Switch 4
const int gpio_sw5_offset = 0x15C; // Offset for Switch 5
const int gpio_sw6_offset = 0x160; // Offset for Switch 6
const int gpio_sw7_offset = 0x164; // Offset for Switch 7
const int gpio_sw8_offset = 0x168; // Offset for Switch 8

const int gpio_pbtnl_offset = 0x16C; // Offset for left push button
const int gpio_pbtnr_offset = 0x170; // Offset for right push button
const int gpio_pbtnu_offset = 0x174; // Offset for up push button
const int gpio_pbtnd_offset = 0x178; // Offset for down push button
const int gpio_pbtnc_offset = 0x17C; // Offset for center push button



class ZedBoard
{
  char *ptr;
  int fd;

  public:
    // Constructor
    ZedBoard() {
			fd = open( "/dev/mem", O_RDWR);
			ptr = (char *) mmap(NULL,
																gpio_size,
																PROT_READ | PROT_WRITE,
																MAP_SHARED,
																fd,
																gpio_address);

    }

		// Destructor
    ~ZedBoard(){
			munmap(ptr, gpio_size);
			close(fd);
    }


    void RegisterWrite(int offset, int value){
			* (int *) (ptr + offset) = value;
    }

    int RegisterRead(int offset){
			return * (int *) (ptr + offset);
    }

		int CheckError(){
			if (ptr == MAP_FAILED)
			{
				perror("Mapping I/O memory failed - Did you run with 'sudo'?\n");
				return -1;
			}
			return 1;
		}
};




/**
 * Convert current state of switches into binary representation
 *
 * @param zb Representation of the Zedboard
 */
int ReadSwitchState(ZedBoard *zb)
{
	int result = 0;

	result += zb->RegisterRead(gpio_sw1_offset);
	result += zb->RegisterRead(gpio_sw2_offset) << 1;
	result += zb->RegisterRead(gpio_sw3_offset) << 2;
	result += zb->RegisterRead(gpio_sw4_offset) << 3;
	result += zb->RegisterRead(gpio_sw5_offset) << 4;
	result += zb->RegisterRead(gpio_sw6_offset) << 5;
	result += zb->RegisterRead(gpio_sw7_offset) << 6;
	result += zb->RegisterRead(gpio_sw8_offset) << 7;

	return result;
}

/**
 * Show lower 8 bits of integer value on LEDs
 *
 * @param zb Representation of the Zedboard
 * @param value Value to show on LEDs
 */
void SetLedNumber(ZedBoard *zb, int value)
{
	zb->RegisterWrite(gpio_led1_offset, value % 2);
	zb->RegisterWrite(gpio_led2_offset, (value / 2) % 2);
	zb->RegisterWrite(gpio_led3_offset, (value / 4) % 2);
	zb->RegisterWrite(gpio_led4_offset, (value / 8) % 2);
	zb->RegisterWrite(gpio_led5_offset, (value / 16) % 2);
	zb->RegisterWrite(gpio_led6_offset, (value / 32) % 2);
	zb->RegisterWrite(gpio_led7_offset, (value / 64) % 2);
	zb->RegisterWrite(gpio_led8_offset, (value / 128) % 2);
}


int PushButtonGet(ZedBoard *zb, int *button_states)
{
	int result;

	if (zb->RegisterRead(gpio_pbtnu_offset) && !button_states[0]){
		result = 1;
	}
	else if (zb->RegisterRead(gpio_pbtnd_offset)  && !button_states[1]){
	        result = 2;
        }
	else if (zb->RegisterRead(gpio_pbtnr_offset)  && !button_states[2]){
	        result = 3;
        }
	else if (zb->RegisterRead(gpio_pbtnl_offset)  && !button_states[3]){
					result = 4;
        }
	else if (zb->RegisterRead(gpio_pbtnc_offset)  && !button_states[4]){
	        result = 5;
        }
	else {
		result = 0;
	}

	button_states[0] = zb->RegisterRead(gpio_pbtnu_offset);
	button_states[1] = zb->RegisterRead(gpio_pbtnd_offset);
	button_states[2] = zb->RegisterRead(gpio_pbtnr_offset);
	button_states[3] = zb->RegisterRead(gpio_pbtnl_offset);
	button_states[4] = zb->RegisterRead(gpio_pbtnc_offset);

	return result;
}

int main()
{
	int button_states[5];
	int counter = 0;
  int tick_rate = 0;
  time_t tick_start = time(NULL);

	ZedBoard *zb = new ZedBoard();
	// Check error

	if (zb->CheckError() == -1){
		return -1;
	}

	while(1)
	{
		int command = PushButtonGet(zb, button_states);

		switch (command){
			case 0:
				break;
			case 1:
				std::cout << "Up" << std::endl;
				tick_rate++;
				break;
			case 2:
				std::cout << "Down" << std::endl;
				tick_rate--;
				break;
			case 3:
				std::cout << "Right" << std::endl;
				if (tick_rate < 0){
          tick_rate = tick_rate * -1;
        }
				break;
			case 4:
				std::cout << "Left" << std::endl;
        if (tick_rate > 0){
          tick_rate = tick_rate * -1;
        }
				break;
			case 5:
				std::cout << "Center" << std::endl;
				counter = ReadSwitchState(zb);
				break;
		}

    if (difftime(tick_start, time(NULL)) > 1){
      tick_start = time(NULL);
      counter += tick_rate;
      if (counter > 255 || counter < 0)
        counter = 0;
    }

		SetLedNumber(zb, counter);
	}

	// Done
	delete zb;
	return 0;
}
