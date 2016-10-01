#include "ZedBoard.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

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

// Constructor
ZedBoard::ZedBoard(){
  fd = open( "/dev/mem", O_RDWR);
  ptr = (char *) mmap(NULL,
                            gpio_size,
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED,
                            fd,
                            gpio_address);

}

// Destructor
ZedBoard::~ZedBoard(){
  munmap(ptr, gpio_size);
  close(fd);
}


void ZedBoard::RegisterWrite(int offset, int value){
  * (int *) (ptr + offset) = value;
}

int ZedBoard::RegisterRead(int offset){
  return * (int *) (ptr + offset);
}

void ZedBoard::setLed(int led, int value){
  switch (led){
    case 0:
      RegisterWrite(gpio_led1_offset, value);
      break;
    case 1:
      RegisterWrite(gpio_led2_offset, value);
      break;
    case 2:
      RegisterWrite(gpio_led3_offset, value);
      break;
    case 3:
      RegisterWrite(gpio_led4_offset, value);
      break;
    case 4:
      RegisterWrite(gpio_led5_offset, value);
      break;
    case 5:
      RegisterWrite(gpio_led6_offset, value);
      break;
    case 6:
      RegisterWrite(gpio_led7_offset, value);
      break;
    case 7:
      RegisterWrite(gpio_led8_offset, value);
      break;
  }
}

int ZedBoard::CheckError(){
  if (ptr == MAP_FAILED)
  {
    perror("Mapping I/O memory failed - Did you run with 'sudo'?\n");
    return -1;
  }
  return 1;
}
