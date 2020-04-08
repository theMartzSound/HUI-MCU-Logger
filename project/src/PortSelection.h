#pragma once
#include "rtmidi/RtMidi.h"

/*	PortSelection
*/

// A helper function used to allow the user to select a port.
// - Returns the number of the selected port. (TODO)
// - May throw RtMidiError exceptions.
// - Throws an exception if there are no ports available. (TODO)
// - Throws an exception if the user chooses to create a virtual port (TODO)
bool openMIDIPort(RtMidi* rtmidi);