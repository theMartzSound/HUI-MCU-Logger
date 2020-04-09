# HUI/MCU Logger
A simple C++ console application that translates and logs the HUI and Mackie Control protocol output of your DAW. With it, you can monitor the messages your DAW sends to a connected Mackie Control or HUI device.

I'm developing this utility to help solve a specific circuit optimization problem in my DIY control surface project, the MCon. More information about this project is available at <https://martzsoundandmusic.com/project-mcon-overview/>.

For this application to be useful to you, you must either have both an MCU and HUI connected to your DAW (yikes) or be running emulations of them. I have developed emulations of both in Max/MSP. If you'd like to use them, they are available to download [here](http://martzsoundandmusic.com/wp-content/uploads/2019/09/HUI_Emulation.zip) and [here](http://martzsoundandmusic.com/wp-content/uploads/2019/09/MCU_EmulationPatcher.zip).

This utility connects to both protocols simultaneously, as the final MCon will.  The purpose of this tool is to allow me to more clearly identify how DAWs group their output messages in these protocols, which will allow me to make informed decisions on how best to optimize my demultiplexing circuits.  Therefore, it may skip handling messages which aren't relevant to LED, display, or motor output.

---

## Getting Started

1. Use Git to clone the repository:

`git clone --recurse-submodules https://github.com/theMartzSound/HUI-MCU-Logger.git`

2. Install **Boost library 1.72.0**, if you haven't already.
3. In the file *premake5.lua*, add the path to your Boost directory to **line 8**.
4. To build the solution and project, run the appropriate script in */scripts/* (currently, there is only one).

Note that this repository does make use of a submodule, so don't forget to use `--recurse-submodules`.  If you do forget, your local repo will be missing files, and you'll receive compilation errors when you build.

## The Plan
At the time of this writing, the logger simply prompts you to pick an input and output port for the emulated MCU and HUI devices, then logs each MIDI message in hexadecimal until you press Enter.

When development is finished, the application will translate and log the messages in plain English.  The point of the tool is to allow me to more clearly identify how these protocols group their output messages, which will allow me to make informed decisions on how best to optimize my output circuits.

## Compatibility
* Windows 64-bit with Visual Studio 2019 (in progress)

This is a very small, single-use app, so I'm probably only going to finish it on Windows.  However, it's just a simple console app made with exclusively multi-platform libraries, so it shouldn't require much modification to build on macOS or Linux.

### Libraries Used
* Premake 5.0: <https://premake.github.io/>
* RtMidi 4.0.0: <https://www.music.mcgill.ca/~gary/rtmidi/>
* Boost 1.72.0: <https://www.boost.org/>