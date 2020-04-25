#include "InterpretMCU.h"

#include <iostream>
#include <fstream>

#include <chrono>
#include <iomanip>
#include <sstream>

namespace MCU_MIDI
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

	void NoteOn(unsigned int channel, unsigned int notenumber, unsigned int velocity)
	{
		updateTime(); //update timestamp
		logFile.open(LOGPATH, std::ios_base::app);

		switch (velocity)
		{
		case 0x00:
			logFile << "MCU," << now_sstream.str() << ",LED Off," << notenumber << std::endl;
			break;
		case 0x01:
			logFile << "MCU," << now_sstream.str() << ",LED Blinking," << notenumber << std::endl;
			break;
		case 0x7F:
			logFile << "MCU," << now_sstream.str() << ",LED On," << notenumber << std::endl;
			break;
		}
	
		logFile.close();
	}

	void CC(unsigned int channel, unsigned int ccnumber, unsigned int value)
	{
		updateTime(); //update timestamp
		logFile.open(LOGPATH, std::ios_base::app);

		if (ccnumber & 0x30)
			logFile << "MCU," << now_sstream.str() << ",V-Pot Ring," << (ccnumber & 0x0F) << "," << value << std::endl;
		else if (ccnumber & 0x40)
			logFile << "MCU," << now_sstream.str() << ",Timecode Character," << (ccnumber & 0x0F) << "," << value << std::endl;
	
		logFile.close();
	}

	void ChannelPressure(unsigned int channel, unsigned int value)
	{
		updateTime(); //update timestamp
		logFile.open(LOGPATH, std::ios_base::app);

		logFile << "MCU," << now_sstream.str() << ",VU Lamp," << (value & 0x70 >> 4) << "," << (value & 0x0F) << std::endl;
	
		logFile.close();
	}

	void PitchBend(unsigned int channel, unsigned int value)
	{
		updateTime(); //update timestamp
		logFile.open(LOGPATH, std::ios_base::app);

		unsigned int faderVal = value >> 4; //Faders are 10-bit, and PB is 14-bit. Rightmost four bits are discarded
		logFile << "MCU," << now_sstream.str() << ",Fader Motor," << channel << "," << faderVal << std::endl;
	
		logFile.close();
	}

	void SysEx()
	{
		// unused so far
		//updateTime(); //update timestamp
		//logFile.open(LOGPATH, std::ios_base::app);
		//logFile << "MCU," << timeString() << ",SysEx" << std::endl;
		//logFile.close();
	}
}