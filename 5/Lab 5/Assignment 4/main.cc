#include "Wiimote.h"
#include "ZedBoard.h"
#include <stdlib.h>

class WiimoteToLed : public Wiimote{

  ZedBoard *zb;

public:

  WiimoteToLed(ZedBoard *zed_board)
      : Wiimote()
  {
    zb = zed_board;
  }

  void AccelerationEvent(int code, int acceleration){
    int amount_leds;
    // Check if x-axis
    if (code != 3){
      return;
    }
    amount_leds = abs(acceleration);
    if (amount_leds > 100){
      amount_leds = 100;
    }
    amount_leds = amount_leds / 12;

    for (int i = 0; i < 8; i++){
      if (amount_leds > 0){
          zb->setLed(i, 1);
          amount_leds--;
      }
      else {
        zb->setLed(i, 0);
      }
    }
  }
};

int main()
{
 // Instantiate ZedBoard object statically
 ZedBoard zed_board;

 // Instantiate WiimoteToLed object statically, passing a pointer to the
 // recently created ZedBoard object.
 WiimoteToLed wiimote_to_led(&zed_board);
 // Enter infinite loop listening to events. The overridden function
 // WiimoteToLed::AccelerationEvent() will be invoked when the user moves
 // the Wiimote.
 wiimote_to_led.Listen();
 // Unreachable code, previous function has an infinite loop
 return 0;
}
