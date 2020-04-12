#pragma once
#include <exception>
#include "rtmidi/RtMidi.h"

/*	PortSelection
*/

// Thrown when no MIDI ports are available.
class NoPorts : public std::exception
{
public:
	virtual const char* what() const throw() = 0;
};

// Thrown when no input MIDI ports are available.
class NoInputPorts : public NoPorts
{
public:
    virtual const char* what() const throw();
};

// Thrown when no output MIDI ports are available.
class NoOutputPorts : public NoPorts
{
public:
    virtual const char* what() const throw();
};

// Prompts the user to pick a port and opens it.
// - On Unix systems, optionally allows user to generate a virtual port.
// - May throw RtMidiError exceptions.
// - Throws a NoPorts exception if there are no ports available.
void openMIDIPort(RtMidi* rtmidi);