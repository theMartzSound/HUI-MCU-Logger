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
    filter "platforms:Win64"        architecture "x86_64"
    -- filter "platforms:Win32"        architecture "x86"
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

    -- Source files
    files { "project/src/**.h", "project/src/**.cpp" }

    -- Build locations
    targetdir ("bin/" .. outputdir)
    objdir ("bin-int/" .. outputdir)