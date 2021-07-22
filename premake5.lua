workspace "Atlas"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Atlas/vendor/GLFW/include"
IncludeDir["Glad"] = "Atlas/vendor/Glad/include"
IncludeDir["ImGui"] = "Atlas/vendor/ImGui"

include "Atlas/vendor/GLFW"
include "Atlas/vendor/Glad"
include "Atlas/vendor/ImGui"

project "Atlas"
	location "Atlas"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "atlpch.h"
	pchsource "Atlas/src/atlpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"ATL_PLATFORM_WINDOWS",
			"ATL_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "ATL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "ATL_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "ATL_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Atlas/vendor/spdlog/include",
		"Atlas/src"
	}

	links
	{
		"Atlas"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"ATL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "ATL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "ATL_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "ATL_DIST"
		optimize "On"