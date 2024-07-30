-- ConfigureCatolYeah-External.lua

-- Include directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"]      = "CatolYeah/vendor/GLFW/include"
IncludeDir["Glad"]      = "CatolYeah/vendor/Glad/include"
IncludeDir["ImGui"]     = "CatolYeah/vendor/imgui/"
IncludeDir["glm"]       = "CatolYeah/vendor/glm/"
IncludeDir["stb_image"] = "CatolYeah/vendor/stb_image/"
IncludeDir["spdlog"]    = "CatolYeah/vendor/spdlog/include/"

group "Dependencies"
    include "CatolYeah/vendor/GLFW"
    include "CatolYeah/vendor/Glad"
    include "CatolYeah/vendor/imgui"
group ""

group "Core"
    include "CatolYeah/Configure-CatolYeah.lua"
group ""