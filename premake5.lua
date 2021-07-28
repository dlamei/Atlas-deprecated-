workspace "Atlas"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Atlas/vendor/GLFW/include"
IncludeDir["Glad"] = "Atlas/vendor/Glad/include"
IncludeDir["ImGui"] = "Atlas/vendor/ImGui"
IncludeDir["GLM"] = "Atlas/vendor/GLM"

group "Dependencies" 
	include "Atlas/vendor/GLFW"
	include "Atlas/vendor/Glad"
	include "Atlas/vendor/ImGui"
group""


project "Atlas"
	location "Atlas"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "atlpch.h"
	pchsource "Atlas/src/atlpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.GLM}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ATL_PLATFORM_WINDOWS",
			"ATL_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines "ATL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ATL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "ATL_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Atlas/src",
		"Atlas/vendor",
		"%{IncludeDir.GLM}"
	}

	links
	{
		"Atlas"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ATL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "ATL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ATL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "ATL_DIST"
		runtime "Release"
		optimize "on"
