#include "InterpretMIDI.h"
#include "rtmidi/RtMidi.h"

static std::vector<unsigned char> HUImessage;
void handleHUIInbound(RtMidiIn* port)
{
	port->getMessage(&HUImessage);
	if (HUImessage.empty())
		return;
	else
	{
		// Print message
		//TODO: print timestamp
		std::cout << "HUI: ";
		for (auto it = HUImessage.begin(); it != HUImessage.end(); ++it)
			std::cout << std::hex << (unsigned int)*it << " ";
		std::cout << std::endl;
	}
	// TODO: Use mutex to log from separate threads
    // TODO: Log output to external file
}

static std::vector<unsigned char> MCUmessage;
void handleMCUInbound(RtMidiIn* port)
{
	port->getMessage(&MCUmessage);
	if (MCUmessage.empty())
		return;
	else
	{
		// Print message
		//TODO: print timestamp
		std::cout << "MCU: ";
		for (auto it = MCUmessage.begin(); it != MCUmessage.end(); ++it)
			std::cout << std::hex << (unsigned int)*it << " ";
		std::cout << std::endl;
	}
	// TODO: Use mutex to log from separate threads
    // TODO: Log output to external file
}