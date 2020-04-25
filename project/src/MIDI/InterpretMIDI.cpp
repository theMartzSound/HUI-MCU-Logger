#include "InterpretMIDI.h"
#include <mutex>

#include "InterpretHUI.h"
#include "InterpretMCU.h"

std::mutex mtx;

enum class MIDIRunningStatus
{
	NoteOn,
	PolyphonicKeyPressure,
	CC,
	ChannelPressure,
	PitchBend,
	SysEx
};
const char* MIDIRunningStatus_tostr[] = {
	"NoteOn",
	"PolyphonicKeyPressure",
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
    
	auto it = message->begin();

	if (*it > 0x7F) // Handle status bytes
	{
		switch (*it & 0xF0) //strip out channel numbers
		{
		case 0x80: //Note off (unexpected, handled same as NoteOn)
			HUIchannel = *it & 0x0F;
			HUIRunningStatus = MIDIRunningStatus::NoteOn;
			std::cout << "Warning: Note Off (0x8n) received in HUI port.  Treating as Note On." << std::endl;
			break;
		case 0x90: //Note on (ping)
			HUIchannel = *it & 0x0F;
			HUIRunningStatus = MIDIRunningStatus::NoteOn;
			break;
		case 0xA0: //Polyphonic key pressure (VU meters)
			HUIchannel = *it & 0x0F;
			HUIRunningStatus = MIDIRunningStatus::PolyphonicKeyPressure;
			break;
		case 0xB0: //CC (tons of things)
			HUIchannel = *it & 0x0F;
			HUIRunningStatus = MIDIRunningStatus::CC;
			break;
		case 0xF0: //Sysex (displays)
			if (*it != 0xF0) //Expecting 0xF0
			{
				std::cout << "Warning: Non SysEx System message received in HUI port!  Message: " << std::hex << (unsigned int)*it << " ";
				for (++it; it != message->end(); ++it)
					std::cout << std::hex << (unsigned int)*it << " ";
				std::cout << std::endl;
				mtx.unlock();
				return;
			}
			else
				HUIRunningStatus = MIDIRunningStatus::SysEx;
			break;
		default: //Exception
			std::cout << "Warning: Unhandled message type received in HUI port!  Message: " << std::hex << (unsigned int)*it << " ";
			for (++it; it != message->end(); ++it)
				std::cout << std::hex << (unsigned int)*it << " ";
			std::cout << std::endl;
			mtx.unlock();
			return;
		}
		++it;
	}
	else
		std::cout << "(HUI) Running status used: " << std::hex << MIDIRunningStatus_tostr[(unsigned int)HUIRunningStatus] << std::endl;


	// Handle rest of message
	switch (HUIRunningStatus)
	{
	case MIDIRunningStatus::NoteOn:
		while (it != message->end())
		{
			unsigned int notenumber = *(it++);
			unsigned int velocity = *(it++);
			HUI_MIDI::NoteOn(HUIchannel, notenumber, velocity);
		}
		break;
	case MIDIRunningStatus::PolyphonicKeyPressure:
		while (it != message->end())
		{
			unsigned int notenumber = *(it++);
			unsigned int pressure = *(it++);
			HUI_MIDI::PolyphonicKeyPressure(HUIchannel, notenumber, pressure);
		}
		break;
	case MIDIRunningStatus::CC:
		while (it != message->end())
		{
			unsigned int ccnumber = *(it++);
			unsigned int value = *(it++);
			HUI_MIDI::CC(HUIchannel, ccnumber, value);
		}
		break;
	case MIDIRunningStatus::SysEx:
		// TODO: Handle SysEx
		HUI_MIDI::SysEx();
		//for (; it != message->end(); ++it)
		//	std::cout << std::hex << unsigned int(*it) << " ";
		//std::cout << std::endl;
		break;
	default:
		std::cout << "Warning: unhandled message.  Running status = \"" << MIDIRunningStatus_tostr[(unsigned int)HUIRunningStatus] << "\", bytes: ";
		for (; it != message->end(); ++it)
			std::cout << std::hex << unsigned int(*it) << " ";
		std::cout << std::endl;
		break;
	}

	mtx.unlock();
}


void handleMCUInbound(double deltatime, std::vector< unsigned char >* message, void* userData)
{
	mtx.lock();
	
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
				std::cout << "Warning: Non SysEx System message received in MCU port!  Message: " << std::hex << (unsigned int)*it << " ";
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
			std::cout << "Warning: Unhandled message type received!  Message: " << std::hex << (unsigned int)*it << " ";
			for (++it; it != message->end(); ++it)
				std::cout << std::hex << (unsigned int)*it << " ";
			std::cout << std::endl;
			mtx.unlock();
			return;
		}
		++it;
	}
	else
		std::cout << "(MCU) Running status used: " << std::hex << MIDIRunningStatus_tostr[(unsigned int)MCURunningStatus] << std::endl;


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
		// TODO: Handle SysEx
		MCU_MIDI::SysEx();
		//for (; it != message->end(); ++it)
		//	std::cout << std::hex << unsigned int(*it) << " ";
		//std::cout << std::endl;
		break;
	default:
		std::cout << "Warning: unhandled message.  Running status = \"" << MIDIRunningStatus_tostr[(unsigned int)HUIRunningStatus] << "\", bytes: ";
		for (; it != message->end(); ++it)
			std::cout << std::hex << unsigned int(*it) << " ";
		std::cout << std::endl;
		break;
	}

	mtx.unlock();
}