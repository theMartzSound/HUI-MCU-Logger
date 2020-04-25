#include "InterpretMCU.h"
#include <iostream>
#include <time.h>

namespace MCU_MIDI
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

	void NoteOn(unsigned int channel, unsigned int notenumber, unsigned int velocity)
	{
		updateTime(); //update timestamp

		switch (velocity)
		{
		case 0x00:
			std::cout << timeString << " (MCU) LED Off.  LED ID: " << notenumber << std::endl;
			break;
		case 0x01:
			std::cout << timeString << " (MCU) LED Blinking.  LED ID: " << notenumber << std::endl;
			break;
		case 0x7F:
			std::cout << timeString << " (MCU) LED On.  LED ID: " << notenumber << std::endl;
			break;
		}
	}

	void CC(unsigned int channel, unsigned int ccnumber, unsigned int value)
	{
		updateTime(); //update timestamp

		if (ccnumber & 0x30)
			std::cout << timeString << " (MCU) V-Pot Ring #" << (ccnumber & 0x0F) << ": value = " << value << std::endl;
		else if (ccnumber & 0x40)
			std::cout << timeString << " (MCU) Timecode display character #" << (ccnumber & 0x0F) << ": value = " << value << std::endl;
	}

	void ChannelPressure(unsigned int channel, unsigned int value)
	{
		updateTime(); //update timestamp

		std::cout << timeString << " (MCU) Meter LED #" << (value & 0x70 >> 4) << ": value = " << (value & 0x0F) << std::endl;
	}

	void PitchBend(unsigned int channel, unsigned int value)
	{
		updateTime(); //update timestamp

		unsigned int faderVal = value >> 4; //Faders are 10-bit, and PB is 14-bit. Rightmost four bits are discarded
		std::cout << timeString << " (MCU) Fader #" << channel << ": value = " << faderVal << std::endl;
	}

	void SysEx()
	{
		// unused so far
		updateTime(); //update timestamp
		std::cout << timeString << " (MCU) SysEx: F0 ";
	}
}