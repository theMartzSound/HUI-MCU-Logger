#pragma once

namespace HUI_MIDI
{
	void NoteOn(unsigned int channel, unsigned int notenumber, unsigned int velocity);
	void PolyphonicKeyPressure(unsigned int channel, unsigned int notenumber, unsigned int pressure);
	void CC(unsigned int channel, unsigned int ccnumber, unsigned int value);
	void SysEx();
}