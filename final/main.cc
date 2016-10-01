#include "RoboticWiimote.h"
#include <stdlib.h>
#include <iostream>

int main()
{


	RoboticWiimote *rwm = new RoboticWiimote();

	rwm->Listen();

	return 0;
}
