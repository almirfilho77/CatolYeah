-- Build-CatolYeah.lua

project "CatolYeah"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    staticruntime "off"

    pchheader "cypch.h"
	pchsource "src/cypch.cpp"

    files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
	}

    includedirs
	{
		"src",
        "vendor/spdlog/include",
        "vendor/GLFW/include",
		--"%{IncludeDir.GLFW}",
        "vendor/Glad/include",
		--"%{IncludeDir.Glad}",
        "vendor/imgui",
		--"%{IncludeDir.ImGui}",
        "vendor/glm/",
		--"%{IncludeDir.glm}",
        "vendor/stb_image/",
		--"%{IncludeDir.stb_image}",
	}

    links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
	}

    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    defines 
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

    filter "system:windows"
		systemversion "latest"

		defines
		{
			"CY_PLATFORM_WINDOWS",
			"CY_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"CY_ASSERT_ENABLED",
		}

	filter "configurations:Debug"
		defines "CY_CONFIG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CY_CONFIG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CY_CONFIG_DIST"
		runtime "Release"
		optimize "on"