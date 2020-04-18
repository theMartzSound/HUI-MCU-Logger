#include "InterpretHUI.h"
#include <iostream>

unsigned int CurrentZone;

unsigned int OperatingFaderNo, PendingMSB, PendingLSB;
bool ReceivedFaderMSB = false, ReceivedFaderLSB = false;

void HUI_MIDI::NoteOn(unsigned int channel, unsigned int notenumber, unsigned int velocity)
{
	if (notenumber == 0x00 && velocity == 0x00)
		std::cout << "(HUI) Ping received" << std::endl;
	else
		std::cout << "(HUI) Error: Non-ping Note On received in HUI port.  Note number = " << notenumber << ", velocity = " << velocity << "." << std::endl;
}

void HUI_MIDI::PolyphonicKeyPressure(unsigned int channel, unsigned int notenumber, unsigned int pressure)
{
	std::cout << "(HUI) VU Meter #" << notenumber << ": ";
	if (pressure & 0x10)
		std::cout << "Left  ";
	else
		std::cout << "Right ";
	std::cout << (pressure & 0x0F) << std::endl;
}

void HUI_MIDI::CC(unsigned int channel, unsigned int ccnumber, unsigned int value)
{
	switch (ccnumber & 0xF0)
	{
	case 0x00: // Zone select (LEDs) or MSB (Faders)
		if (ccnumber == 0x0C) // LEDs
			CurrentZone = value;
		else // Faders
		{
			PendingMSB = value;

			if (ReceivedFaderMSB)
			{
				ReceivedFaderLSB == false;
				std::cout << "(HUI) Warning: a fader MSB was unpaired.  Fader no: " << OperatingFaderNo << std::endl;
				OperatingFaderNo = ccnumber & 0x0F;
			}
			else
			{
				OperatingFaderNo = ccnumber & 0x0F;
				if (ReceivedFaderLSB)
				{
					ReceivedFaderLSB = false;
					std::cout << "(HUI) Fader #" << OperatingFaderNo << ": value = " << ((PendingMSB << 3) + (PendingLSB >> 4)) << std::endl;
				}
				else
					ReceivedFaderMSB = true;
			}
		}
		break;
	case 0x10: // V-Pot rings
		std::cout << "(HUI) V-Pot #" << (ccnumber & 0x0F) << ": configuration =" << value << std::endl;
		break;
	case 0x20: // Switch port (LEDs) or LSB (Faders)
		if (ccnumber == 0x2C)
		{
			if (value & 0x40)
				std::cout << "(HUI) LED On:  zone = " << CurrentZone << ", port = " << (value & 0x0F) << std::endl;
			else
				std::cout << "(HUI) LED Off: zone = " << CurrentZone << ", port = " << (value & 0x0F) << std::endl;
		}
		else // Faders
		{
			PendingLSB = value;

			if (ReceivedFaderLSB)
			{
				ReceivedFaderMSB == false;
				std::cout << "(HUI) Warning: a fader LSB was unpaired.  Fader no: " << OperatingFaderNo << std::endl;
				OperatingFaderNo = ccnumber & 0x0F;
			}
			else
			{
				OperatingFaderNo = ccnumber & 0x0F;
				if (ReceivedFaderMSB)
				{
					ReceivedFaderMSB = false;
					std::cout << "(HUI) Fader #" << OperatingFaderNo << ": value = " << ((PendingMSB << 3) + (PendingLSB >> 4)) << std::endl;
				}
				else
					ReceivedFaderLSB = true;
			}
		}
		break;
	default:
		std::cout << "(HUI) Error: Unhandled CC message in HUI port.  CC = " << ccnumber << ", value = " << value << std::endl;
		break;
	}
}

void HUI_MIDI::SysEx()
{
}
