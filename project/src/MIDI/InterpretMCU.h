#pragma once

namespace MCU_MIDI
{
	void NoteOn(unsigned int channel, unsigned int notenumber, unsigned int velocity);
	void CC(unsigned int channel, unsigned int ccnumber, unsigned int value);
	void ChannelPressure(unsigned int channel, unsigned int value);
	void PitchBend(unsigned int channel, unsigned int value);
	void SysEx();
}