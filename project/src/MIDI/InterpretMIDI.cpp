#include "InterpretMIDI.h"
#include <mutex>

#include "InterpretMCU.h"

std::mutex mtx;

enum class MIDIRunningStatus
{
	NoteOn,
	CC,
	ChannelPressure,
	PitchBend,
	SysEx
};
const char* MIDIRunningStatus_tostr[] = {
	"NoteOn",
	"CC",
	"ChannelPressure",
	"PitchBend",
	"SysEx"
};
MIDIRunningStatus HUIRunningStatus;
unsigned int HUIchannel;
MIDIRunningStatus MCURunningStatus;
unsigned int MCUchannel;

void handleHUIInbound(double deltatime, std::vector< unsigned char >* message, void* userData)
{
	mtx.lock();
	
	//TODO: translate HUI
	std::cout << "HUI: ";
	for (auto it = message->begin(); it != message->end(); ++it)
		std::cout << std::hex << (unsigned int)*it << " ";
	std::cout << std::endl;
    
	// TODO: Log output to external file

	mtx.unlock();
}

void handleMCUInbound(double deltatime, std::vector< unsigned char >* message, void* userData)
{
	mtx.lock();

	std::cout << "(MCU) ";

	//for (auto it = message->begin(); it != message->end(); ++it)
	//	std::cout << std::hex << (unsigned int)*it << " ";
	//std::cout << std::endl;
	
	auto it = message->begin();

	if (*it > 0x7F) // Handle status bytes
	{
		switch (*it & 0xF0) //strip out channel numbers
		{
		case 0x80: //Note off (unexpected, handled same as NoteOn)
			MCUchannel = *it & 0x0F;
			MCURunningStatus = MIDIRunningStatus::NoteOn;
			std::cout << "Warning: Note Off (0x8n) received in MCU port.  Treating as Note On." << std::endl;
			break;
		case 0x90: //Note on (buttons/leds)
			MCUchannel = *it & 0x0F;
			MCURunningStatus = MIDIRunningStatus::NoteOn;
			break;
		case 0xB0: //CC (V-Pots, individual timecode characters)
			MCUchannel = *it & 0x0F;
			MCURunningStatus = MIDIRunningStatus::CC;
			break;
		case 0xD0: //Channel Pressure (Meter bridge, unused by MCon)
			MCUchannel = *it & 0x0F;
			MCURunningStatus = MIDIRunningStatus::ChannelPressure;
			break;
		case 0xE0: //Pitch Bend (Fader positions)
			MCUchannel = *it & 0x0F;
			MCURunningStatus = MIDIRunningStatus::PitchBend;
			break;
		case 0xF0: //Sysex (tons of things)
			if (*it != 0xF0) //Expecting 0xF0
			{
				std::cout << "Error: Non SysEx System message received in MCU port!  Message: " << std::hex << (unsigned int)*it << " ";
				for (++it; it != message->end(); ++it)
					std::cout << std::hex << (unsigned int)*it << " ";
				std::cout << std::endl;
				mtx.unlock();
				return;
			}
			else
				MCURunningStatus = MIDIRunningStatus::SysEx;
			break;
		default: //Exception
			std::cout << "Error: Unhandled message type received!  Message: " << std::hex << (unsigned int)*it << " ";
			for (++it; it != message->end(); ++it)
				std::cout << std::hex << (unsigned int)*it << " ";
			std::cout << std::endl;
			mtx.unlock();
			return;
		}
		++it;
	}
	else
		std::cout << "Running status used: " << std::hex << MIDIRunningStatus_tostr[(unsigned int)MCURunningStatus] << std::endl;


	// Handle rest of message
	switch (MCURunningStatus)
	{
	case MIDIRunningStatus::NoteOn:
		while (it != message->end())
		{
			unsigned int notenumber = *(it++);
			unsigned int velocity = *(it++);
			MCU_MIDI::NoteOn(MCUchannel, notenumber, velocity);
		}
		break;
	case MIDIRunningStatus::CC:
		while (it != message->end())
		{
			unsigned int ccnumber = *(it++);
			unsigned int value = *(it++);
			MCU_MIDI::CC(MCUchannel, ccnumber, value);
		}
		break;
	case MIDIRunningStatus::ChannelPressure:
		while (it != message->end())
		{
			unsigned int value = *(it++);
			MCU_MIDI::ChannelPressure(MCUchannel, value);
		}
		break;
	case MIDIRunningStatus::PitchBend:
		while (it != message->end())
		{
			unsigned int value = *(it++); //LSB
			value += *(it++) << 7; //MSB
			MCU_MIDI::PitchBend(MCUchannel, value);
		}
		break;
	case MIDIRunningStatus::SysEx:
		std::cout << "SysEx: F0 ";
		// TODO: Handle SysEx

		for (; it != message->end(); ++it)
			std::cout << std::hex << unsigned int(*it) << " ";
		std::cout << std::endl;
		break;
	default:
		break;
	}

    // TODO: Log output to external file

	mtx.unlock();
}