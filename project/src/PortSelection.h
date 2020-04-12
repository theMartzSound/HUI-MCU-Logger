#pragma once
#include <exception>
#include "rtmidi/RtMidi.h"

/*	PortSelection
*/

// A helper function used to allow the user to select a port.
// - Returns the number of the selected port. (TODO)
// - May throw RtMidiError exceptions.
// - Throws an exception if there are no ports available. (TODO)
// - Throws an exception if the user chooses to create a virtual port (TODO)

class NoPorts : public std::exception
{
public:
	virtual const char* what() const throw() = 0;
};

class NoInputPorts : public NoPorts
{
public:
    virtual const char* what() const throw();
};
class NoOutputPorts : public NoPorts
{
public:
    virtual const char* what() const throw();
};

void openMIDIPort(RtMidi* rtmidi);