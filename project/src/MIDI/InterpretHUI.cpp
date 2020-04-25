#include "InterpretHUI.h"

#include <iostream>
#include <fstream>

#include <chrono>
#include <iomanip>
#include <sstream>

namespace HUI_MIDI
{
	std::ofstream logFile;
	#define LOGPATH "log.csv"

	std::chrono::system_clock::time_point now_tp;
	std::chrono::milliseconds now_ms;
	std::time_t now_c;
	std::stringstream now_sstream;

	void updateTime()
	{
		now_tp = std::chrono::system_clock::now();
		now_c = std::chrono::system_clock::to_time_t(now_tp);
		now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now_tp.time_since_epoch()) % 1000;

		now_sstream.str(std::string()); //clear contents
		now_sstream.clear(); //reset error flags
		now_sstream << std::put_time(std::localtime(&now_c), "%T:") << now_ms.count();
	}

	unsigned int CurrentZone;

	unsigned int OperatingFaderNo, PendingMSB, PendingLSB;
	bool ReceivedFaderMSB = false, ReceivedFaderLSB = false;

	void NoteOn(unsigned int channel, unsigned int notenumber, unsigned int velocity)
	{
		updateTime(); //update timestamp
		logFile.open(LOGPATH, std::ios_base::app);

		if (notenumber == 0x00 && velocity == 0x00) {}
			//logFile << "HUI," << now_sstream.str() << ",Ping received" << std::endl;
		else
			std::cout << "[" << now_sstream.str() << "] (HUI) Error: Non-ping Note On received.  Note number = " << notenumber << ", velocity = " << velocity << "." << std::endl;
		
		logFile.close();
	}

	void PolyphonicKeyPressure(unsigned int channel, unsigned int notenumber, unsigned int pressure)
	{
		updateTime(); //update timestamp
		logFile.open(LOGPATH, std::ios_base::app);

		logFile << "HUI," << now_sstream.str() << ",VU Meter," << notenumber;
		if (pressure & 0x10)
			logFile << ",L" << (pressure & 0x0F) << std::endl;
		else
			logFile << ",R" << (pressure & 0x0F) << std::endl;
	
		logFile.close();
	}

	void CC(unsigned int channel, unsigned int ccnumber, unsigned int value)
	{
		updateTime(); //update timestamp
		logFile.open(LOGPATH, std::ios_base::app);

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
					ReceivedFaderLSB = false;
					std::cout << "[" << now_sstream.str() << "] (HUI) Error: Unpaired fader MSB.  Fader no: " << OperatingFaderNo << std::endl;
					OperatingFaderNo = ccnumber & 0x0F;
				}
				else
				{
					OperatingFaderNo = ccnumber & 0x0F;
					if (ReceivedFaderLSB)
					{
						ReceivedFaderLSB = false;
						logFile << "HUI," << now_sstream.str() << ",Fader Motor," << OperatingFaderNo << "," << ((PendingMSB << 3) + (PendingLSB >> 4)) << std::endl;
					}
					else
						ReceivedFaderMSB = true;
				}
			}
			break;
		case 0x10: // V-Pot rings
			logFile << "HUI," << now_sstream.str() << ",V-Pot Ring," << (ccnumber & 0x0F) << "," << value << std::endl;
			break;
		case 0x20: // Switch port (LEDs) or LSB (Faders)
			if (ccnumber == 0x2C)
			{
				if (value & 0x40)
					logFile << "HUI," << now_sstream.str() << ",LED On,Z" << CurrentZone << "P" << (value & 0x0F) << std::endl;
				else
					logFile << "HUI," << now_sstream.str() << ",LED Off,Z" << CurrentZone << "P" << (value & 0x0F) << std::endl;
			}
			else // Faders
			{
				PendingLSB = value;

				if (ReceivedFaderLSB)
				{
					ReceivedFaderMSB = false;
					std::cout << "[" << now_sstream.str() << "] (HUI) Error: Unpaired Fader LSB.  Fader no: " << OperatingFaderNo << std::endl;
					OperatingFaderNo = ccnumber & 0x0F;
				}
				else
				{
					OperatingFaderNo = ccnumber & 0x0F;
					if (ReceivedFaderMSB)
					{
						ReceivedFaderMSB = false;
						logFile << "HUI," << now_sstream.str() << ",Fader Motor," << OperatingFaderNo << "," << ((PendingMSB << 3) + (PendingLSB >> 4)) << std::endl;
					}
					else
						ReceivedFaderLSB = true;
				}
			}
			break;
		default:
			std::cout << "[" << now_sstream.str() << "] (HUI) Error: Unhandled CC message.  CC = " << ccnumber << ", value = " << value << std::endl;
			break;
		}
	
		logFile.close();
	}

	void SysEx()
	{
		// unused so far
		//updateTime(); //update timestamp
		//logFile.open(LOGPATH, std::ios_base::app);
		//logFile << "HUI," << now_sstream.str() << "," << ",SysEx" << std::endl;
		//logFile.close();
	}
}