#include "text.h"
int main()
{
	uint16_t a = 0;
	client objiective(17804, "192.168.3.137");
	objiective.subscribe(a, 0xaf, 100, 60000);
	objiective.pause_action(a, 0x00, 0x08, 0x00);
	objiective.resume_action(a, 0x02, 0x09, 0x10, 0x11);
	objiective.cancel_action(a, 0x00, 0x10, 0x10, 0x01);
	objiective.pallet_action(a, 0x02, 0x11, 0x00);
	//service myService(17804, "192.168.3.125");

	return 0;
}