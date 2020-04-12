#include "InterpretMIDI.h"
#include <mutex>

std::mutex mtx;

void handleHUIInbound(double deltatime, std::vector< unsigned char >* message, void* userData)
{
	mtx.lock();
	//TODO: print timestamp
	std::cout << "HUI: ";
	for (auto it = message->begin(); it != message->end(); ++it)
		std::cout << std::hex << (unsigned int)*it << " ";
	std::cout << std::endl;
	mtx.unlock();
    // TODO: Log output to external file
}

void handleMCUInbound(double deltatime, std::vector< unsigned char >* message, void* userData)
{
	mtx.lock();
	// TODO: print timestamp
	std::cout << "MCU: ";
	for (auto it = message->begin(); it != message->end(); ++it)
		std::cout << std::hex << (unsigned int)*it << " ";
	std::cout << std::endl;
	mtx.unlock();
    // TODO: Log output to external file
}