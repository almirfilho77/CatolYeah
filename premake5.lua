-- premake5.lua

workspace "CatolYeah"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"] = "CatolYeah/vendor/GLFW/include"
IncludeDir["Glad"] = "CatolYeah/vendor/Glad/include"
IncludeDir["ImGui"] = "CatolYeah/vendor/imgui/"
IncludeDir["glm"] = "CatolYeah/vendor/glm/"

-- Similar to "add_subdirectory" of CMake
include "CatolYeah/vendor/GLFW"
include "CatolYeah/vendor/Glad"
include "CatolYeah/vendor/imgui"

project "CatolYeah"
	location "CatolYeah"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "cypch.h"
	pchsource "CatolYeah/src/cypch.cpp"

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
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "off"
		systemversion "latest"

		defines
		{
			"CY_PLATFORM_WINDOWS",
			"CY_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "CY_CONFIG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CY_CONFIG_RELEASE"
		runtime "Release"
		symbols "Off"
		optimize "On"

	filter "configurations:Dist"
		defines "CY_CONFIG_DIST"
		runtime "Release"
		symbols "Off"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"CatolYeah/vendor/spdlog/include",
		"CatolYeah/src",
		"%{IncludeDir.glm}"
	}

	links
	{
		"CatolYeah"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "off"
		systemversion "latest"

		defines
		{
			"CY_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "CY_CONFIG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CY_CONFIG_RELEASE"
		runtime "Release"
		symbols "Off"
		optimize "On"

	filter "configurations:Dist"
		defines "CY_CONFIG_DIST"
		runtime "Release"
		symbols "Off"
		optimize "On"