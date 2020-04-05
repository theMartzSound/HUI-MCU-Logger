--[[
    HUI/MCU Output Logger
    Utilized by Premake to generate project and solution files.
    To generate projects and solutions, run the appropriate script in _Scripts.
    Currently only supports Windows on Visual Studio 2019.
]]

workspace "HUI_MCU_OutputLogger"
    configurations { "Debug", "Release" }
    platforms { "Win64" } -- , "Win32" }

    filter "configurations:Debug"   defines { "DEBUG" }     symbols  "On"
    filter "configurations:Release" defines { "RELEASE" }   optimize "On"

    filter "platforms:Win64"        architecture "x86_64"   defines  "__WINDOWS_MM__"
    -- filter "platforms:Win32"        architecture "x86"      defines  "__WINDOWS_MM__"
    filter {}

local outputdir = "%{cfg.shortname}"

project "HUI_MCU_OutputLogger"
    location "project"

    -- Build options
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"

    -- Additional dependencies
    links { "winmm.lib" }

    -- Include directories
    includedirs { "project/vendor" }

    -- Source files
    files 
    {
        "project/src/**.h", 
        "project/src/**.cpp",
        "project/vendor/rtmidi/RtMidi.h",
        "project/vendor/rtmidi/RtMidi.cpp"
    }

    -- Build locations
    targetdir ("bin/" .. outputdir)
    objdir ("bin-int/" .. outputdir)