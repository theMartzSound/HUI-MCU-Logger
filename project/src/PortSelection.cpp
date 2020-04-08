#include <iostream>
#include <string>
#include <sstream>
#include "boost/lexical_cast.hpp"

#include "PortSelection.h"

bool openMIDIPort(RtMidi* rtmidi)
{
    std::string input;

    // Optionally create a virtual MIDI port (Unix only) ------------------------------------------
    #ifndef __WINDOWS_MM__

    while (true)
	{
		std::cout << "Establish a new virtual port? [y/n] ";
		std::getline(std::cin, input);

		if (input == "y" || input == "Y") 
        {
            std::cout << "Enter a port name: ";
            std::getline(std::cin, input);
			rtmidi->openVirtualPort(input.c_str());
            std::cout << "Port \"" << input.c_str() << "\" opened!" << std::endl;
			return true;
		}
        else if (input == "n" || input == "N")
        {
            break;
        }
        std::cout << "Invalid input.  Please respond with [y/n] selection." << std::endl;
	}
    #endif

    // Choose an existing port --------------------------------------------------------------------
    unsigned int portCount = rtmidi->getPortCount();
    unsigned int i = -1;

    // No ports available
    if (portCount == 0) {
        if (typeid(*rtmidi) == typeid(RtMidiIn))
            std::cout << "No input ports detected!" << std::endl;
        else
            std::cout << "No output ports detected!" << std::endl;
        return false;
    }

    std::cout << "------------------------------------" << std::endl;

    // List available ports
	for (unsigned int i = 0; i < portCount; i++)
        std::cout << i << ": " << rtmidi->getPortName(i) << std::endl;

    // Select a port
    while (true)
    {
        std::cout << "Select a port [#]: ";
        std::getline(std::cin, input);

		try {
            i = boost::lexical_cast<int>(input);
            if (i >= portCount)
                std::cout << "Invalid selection.  Pick a valid port." << std::endl;
            else
                break;
		}
        catch (boost::bad_lexical_cast const& e) {
            std::cout << "Invalid input.  Enter the number of the port to open." << std::endl;
        }
    }

    // Victory message
    rtmidi->openPort(i);
    std::cout << "Port \"" << rtmidi->getPortName(i) << "\" opened!" << std::endl;
    return true;
}
