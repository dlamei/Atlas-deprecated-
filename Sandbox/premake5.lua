project "Sandbox"
	--kind "WindowedApp"
	kind "ConsoleApp"
	--entrypoint "mainCRTStartup"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Atlas/vendor/spdlog/include",
		"%{wks.location}/Atlas/src",
		"%{wks.location}/Atlas/vendor",
		"%{IncludeDir.glm}",
	}

	links
	{
		"Atlas",
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
