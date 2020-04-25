#include "InterpretHUI.h"
#include <iostream>
#include <time.h>

namespace HUI_MIDI
{
	time_t rawTime;
	struct tm* processedTime;
	char timeString[11];
	void updateTime()
	{
		time(&rawTime);
		processedTime = localtime(&rawTime);
		strftime(timeString, 11, "[%T]", processedTime);
	}

	unsigned int CurrentZone;

	unsigned int OperatingFaderNo, PendingMSB, PendingLSB;
	bool ReceivedFaderMSB = false, ReceivedFaderLSB = false;

	void NoteOn(unsigned int channel, unsigned int notenumber, unsigned int velocity)
	{
		updateTime(); //update timestamp

		if (notenumber == 0x00 && velocity == 0x00)
			std::cout << timeString << " (HUI) Ping received" << std::endl;
		else
			std::cout << timeString << " (HUI) Error: Non-ping Note On received in HUI port.  Note number = " << notenumber << ", velocity = " << velocity << "." << std::endl;
	}

	void PolyphonicKeyPressure(unsigned int channel, unsigned int notenumber, unsigned int pressure)
	{
		updateTime(); //update timestamp

		std::cout << timeString << " (HUI) VU Meter #" << notenumber << ": ";
		if (pressure & 0x10)
			std::cout << "Left  ";
		else
			std::cout << "Right ";
		std::cout << (pressure & 0x0F) << std::endl;
	}

	void CC(unsigned int channel, unsigned int ccnumber, unsigned int value)
	{
		updateTime(); //update timestamp

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
					std::cout << timeString << " (HUI) Warning: a fader MSB was unpaired.  Fader no: " << OperatingFaderNo << std::endl;
					OperatingFaderNo = ccnumber & 0x0F;
				}
				else
				{
					OperatingFaderNo = ccnumber & 0x0F;
					if (ReceivedFaderLSB)
					{
						ReceivedFaderLSB = false;
						std::cout << timeString << " (HUI) Fader #" << OperatingFaderNo << ": value = " << ((PendingMSB << 3) + (PendingLSB >> 4)) << std::endl;
					}
					else
						ReceivedFaderMSB = true;
				}
			}
			break;
		case 0x10: // V-Pot rings
			std::cout << timeString << " (HUI) V-Pot #" << (ccnumber & 0x0F) << ": configuration =" << value << std::endl;
			break;
		case 0x20: // Switch port (LEDs) or LSB (Faders)
			if (ccnumber == 0x2C)
			{
				if (value & 0x40)
					std::cout << timeString << " (HUI) LED On:  zone = " << CurrentZone << ", port = " << (value & 0x0F) << std::endl;
				else
					std::cout << timeString << " (HUI) LED Off: zone = " << CurrentZone << ", port = " << (value & 0x0F) << std::endl;
			}
			else // Faders
			{
				PendingLSB = value;

				if (ReceivedFaderLSB)
				{
					ReceivedFaderMSB == false;
					std::cout << timeString << " (HUI) Warning: a fader LSB was unpaired.  Fader no: " << OperatingFaderNo << std::endl;
					OperatingFaderNo = ccnumber & 0x0F;
				}
				else
				{
					OperatingFaderNo = ccnumber & 0x0F;
					if (ReceivedFaderMSB)
					{
						ReceivedFaderMSB = false;
						std::cout << timeString << " (HUI) Fader #" << OperatingFaderNo << ": value = " << ((PendingMSB << 3) + (PendingLSB >> 4)) << std::endl;
					}
					else
						ReceivedFaderLSB = true;
				}
			}
			break;
		default:
			std::cout << timeString << " (HUI) Error: Unhandled CC message in HUI port.  CC = " << ccnumber << ", value = " << value << std::endl;
			break;
		}
	}

	void SysEx()
	{
		// unused so far
		updateTime(); //update timestamp
		std::cout << timeString << " (HUI) SysEx: F0 ";
	}
}