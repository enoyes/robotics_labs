#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

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

/**
 * Write a 4-byte value at the specified general-purpose I/O location.
 *
 * @param ptr Base address returned by 'mmap'.
 * @parem offset Offset where device is mapped.
 * @param value Value to be written.
 */
void RegisterWrite(char *ptr, int offset, int value)
{
	* (int *) (ptr + offset) = value;
}

/**
 * Read a 4-byte value from the specified general-purpose I/O location.
 *
 * @param ptr Base address returned by 'mmap'.
 * @param offset Offset where device is mapped.
 * @return Value read.
 */
int RegisterRead(char *ptr, int offset)
{
	return * (int *) (ptr + offset);
}

/**
 * Initialize general-purpose I/O
 * - Opens access to physical memory /dev/mem
 * - Maps memory at offset 'gpio_address' into virtual address space
 *
 * @param fd File descriptor passed by reference, where the result
 * of function 'open' will be stored.
 * @return Address to virtual memory which is mapped to physical,
 * or MAP_FAILED on error.
 */
char *Initialize(int *fd)
{
	*fd = open( "/dev/mem", O_RDWR);
	return (char *) mmap(
			NULL,
			gpio_size,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			*fd,
			gpio_address);
}

/**
 * Close general-purpose I/O.
 *
 * @param ptr Virtual address where I/O was mapped.
 * @param fd File descriptor previously returned by 'open'.
 */
void Finalize(char *ptr, int fd)
{
	munmap(ptr, gpio_size);
	close(fd);
}

/**
 * Show lower 8 bits of integer value on LEDs
 *
 * @param ptr Base address of I/O
 * @param value Value to show on LEDs
 */
void SetLedStateSwitch(char *ptr)
{
	RegisterWrite(ptr, gpio_led1_offset, RegisterRead(ptr, gpio_sw1_offset));
	RegisterWrite(ptr, gpio_led2_offset, RegisterRead(ptr, gpio_sw2_offset));
	RegisterWrite(ptr, gpio_led3_offset, RegisterRead(ptr, gpio_sw3_offset));
	RegisterWrite(ptr, gpio_led4_offset, RegisterRead(ptr, gpio_sw4_offset));
	RegisterWrite(ptr, gpio_led5_offset, RegisterRead(ptr, gpio_sw5_offset));
	RegisterWrite(ptr, gpio_led6_offset, RegisterRead(ptr, gpio_sw6_offset));
	RegisterWrite(ptr, gpio_led7_offset, RegisterRead(ptr, gpio_sw7_offset));
	RegisterWrite(ptr, gpio_led8_offset, RegisterRead(ptr, gpio_sw8_offset));
}

int ReadSwitchState(char *ptr)
{
	int result = 0;

	result += RegisterRead(ptr, gpio_sw1_offset);
	result += RegisterRead(ptr, gpio_sw2_offset) << 1;
	result += RegisterRead(ptr, gpio_sw3_offset) << 2;
	result += RegisterRead(ptr, gpio_sw4_offset) << 3;
	result += RegisterRead(ptr, gpio_sw5_offset) << 4;
	result += RegisterRead(ptr, gpio_sw6_offset) << 5;
	result += RegisterRead(ptr, gpio_sw7_offset) << 6;
	result += RegisterRead(ptr, gpio_sw8_offset) << 7;

	return result;
}

/**
 * Show lower 8 bits of integer value on LEDs
 *
 * @param ptr Base address of I/O
 * @param value Value to show on LEDs
 */
void SetLedNumber(char *ptr, int value)
{
	RegisterWrite(ptr, gpio_led1_offset, value % 2);
	RegisterWrite(ptr, gpio_led2_offset, (value / 2) % 2);
	RegisterWrite(ptr, gpio_led3_offset, (value / 4) % 2);
	RegisterWrite(ptr, gpio_led4_offset, (value / 8) % 2);
	RegisterWrite(ptr, gpio_led5_offset, (value / 16) % 2);
	RegisterWrite(ptr, gpio_led6_offset, (value / 32) % 2);
	RegisterWrite(ptr, gpio_led7_offset, (value / 64) % 2);
	RegisterWrite(ptr, gpio_led8_offset, (value / 128) % 2);
}

int PushButtonGet(char *ptr, int *button_states)
{
	int result;

	if (RegisterRead(ptr, gpio_pbtnu_offset) && !button_states[0]){
		sleep(0.1);
		result = 1;
	}
	else if (RegisterRead(ptr, gpio_pbtnd_offset)  && !button_states[1]){
        	sleep(0.1);
	        result = 2;
        }
	else if (RegisterRead(ptr, gpio_pbtnr_offset)  && !button_states[2]){
        	sleep(0.1);
	        result = 3;
        }
	else if (RegisterRead(ptr, gpio_pbtnl_offset)  && !button_states[3]){
               	sleep(0.1);
		result = 4;
        }
	else if (RegisterRead(ptr, gpio_pbtnc_offset)  && !button_states[4]){
        	sleep(0.1);
	        result = 5;
        }
	else {
		result = 0;
	}

	button_states[0] = RegisterRead(ptr, gpio_pbtnu_offset);
	button_states[1] = RegisterRead(ptr, gpio_pbtnd_offset);
	button_states[2] = RegisterRead(ptr, gpio_pbtnr_offset);
	button_states[3] = RegisterRead(ptr, gpio_pbtnl_offset);
	button_states[4] = RegisterRead(ptr, gpio_pbtnc_offset);

	return result;
}

int main()
{
	int button_states[5];
	int counter = 0;

	// Initialize
	int fd;
	char *ptr = Initialize(&fd);

	// Check error
	if (ptr == MAP_FAILED)
	{
		perror("Mapping I/O memory failed - Did you run with 'sudo'?\n");
		return -1;
	}

	while(1)
	{
		int command = PushButtonGet(ptr, button_states);

		switch (command){
			case 0:
				break;
			case 1:
				printf("Up");
				counter++;
				break;
			case 2:
				printf("Down");
				counter--;
				break;
			case 3:
				printf("Right");
				counter = counter / 2;
				break;
			case 4:
				printf("Left");
				counter = counter * 2;
				break;
			case 5:
				printf("Center");
				counter = ReadSwitchState(ptr);
				break;
		}

		if (counter > 255)
			counter = 0;

		SetLedNumber(ptr, counter);
	}

	// Done
	Finalize(ptr, fd);
	return 0;
}
