#include "InterpretMCU.h"
#include <iostream>

void MCU_MIDI::NoteOn(unsigned int channel, unsigned int notenumber, unsigned int velocity)
{
	switch (velocity)
	{
	case 0x00:
		std::cout << "LED Off.  LED ID: " << notenumber << std::endl;
		break;
	case 0x01:
		std::cout << "LED Blinking.  LED ID: " << notenumber << std::endl;
		break;
	case 0x7F:
		std::cout << "LED On.  LED ID: " << notenumber << std::endl;
		break;
	}
}

void MCU_MIDI::CC(unsigned int channel, unsigned int ccnumber, unsigned int value)
{
	if (ccnumber & 0x30)
		std::cout << "V-Pot Ring #" << (ccnumber & 0x0F) << ".  Value: " << value << std::endl;
	else if (ccnumber & 0x40)
		std::cout << "Timecode display character #" << (ccnumber & 0x0F) << ".  Value: " << value << std::endl;
}

void MCU_MIDI::ChannelPressure(unsigned int channel, unsigned int value)
{
	std::cout << "Meter Bridge #" << (value & 0x70 >> 4) << ".  Value: " << (value & 0x0F) << std::endl;
}

void MCU_MIDI::PitchBend(unsigned int channel, unsigned int value)
{
	unsigned int faderVal = value >> 4; //Faders are 10-bit, and PB is 14-bit. Rightmost four bits are discarded
	std::cout << "Fader #" << channel << ".  Value: " << faderVal << std::endl;
}

void MCU_MIDI::SysEx()
{
	// unused so far
}
