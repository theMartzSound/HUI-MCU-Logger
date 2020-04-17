#include <iostream>
#include <string>
#include "boost/lexical_cast.hpp"

#include "ConsoleHelpers.h"
#include "PortSelection.h"

inline const char* NoInputPorts::what() const throw() { return "No MIDI input ports were detected!  Aborting..."; }
//inline const char* NoOutputPorts::what() const throw() { return "No MIDI output ports were detected!  Aborting..."; }

void printPortNames(unsigned int portCount, RtMidi* rtmidi)
{
    for (unsigned int i = 0; i < portCount; i++)
        std::cout << i << ": " << rtmidi->getPortName(i) << std::endl;
}

void openMIDIPort(RtMidi* rtmidi)
{
    chelp::hrule();

    #ifndef __WINDOWS_MM__
    // Optionally create a virtual MIDI port (Unix only)
    if (chelp::confirmprompt("Establish a new virtual port? [y/n] "))
    {
        std::string input;
        std::cout << "Enter a port name: ";
        std::getline(std::cin, input);
        rtmidi->openVirtualPort(input.c_str());
        std::cout << "Port \"" << input.c_str() << "\" opened!" << std::endl;
        return;
    }
    #endif

    // Check for available ports
    unsigned int portCount = rtmidi->getPortCount();
    if (portCount == 0) {
        //if (typeid(*rtmidi) == typeid(RtMidiIn))
            throw NoInputPorts();
        //else
        //    throw NoOutputPorts();
    }

    // Pick a port
    printPortNames(portCount, rtmidi);
    unsigned int selection = chelp::picknumber(portCount, "Select a port [#]: ");
    // TODO: Handle if a previously selected port is chosen

    // Open port
    rtmidi->openPort(selection);
    std::cout << "Port \"" << rtmidi->getPortName(selection) << "\" opened!" << std::endl;
}