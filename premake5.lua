include "Dependencies.lua"

workspace "Atlas"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Atlas/vendor/GLFW"
	include "Atlas/vendor/Glad"
	include "Atlas/vendor/imgui"
group ""

include "Atlas"
include "Sandbox"
