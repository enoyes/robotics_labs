#include "Wiimote.h"

int main()
{
	Wiimote *wm = new Wiimote();

	wm->Listen();

	return 0;
}
