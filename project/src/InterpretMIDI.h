#pragma once
#include "rtmidi/RtMidi.h"

void handleHUIInbound(double deltatime, std::vector< unsigned char >* message, void* userData);

void handleMCUInbound(double deltatime, std::vector< unsigned char >* message, void* userData);