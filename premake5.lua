workspace "OGLFramework"

architecture "x64"

configurations
{
	"Debug",
	"Release"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "OGLFramework/vendor/GLFW/include"
IncludeDir["Glad"] = "OGLFramework/vendor/GLAD/include"
IncludeDir["ImGui"] = "OGLFramework/vendor/ImGui/"
IncludeDir["glm"] = "OGLFramework/vendor/glm/"
IncludeDir["stb_image"] = "OGLFramework/vendor/stb_image/"

include "OGLFramework/vendor/GLFW"
include "OGLFramework/vendor/Glad"
include "OGLFramework/vendor/ImGui"

project "OGLFramework"

	location "OGLFramework"
	kind "ConsoleApp"
	staticruntime "on"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/vendor/glm/glm/**.hpp",
			"%{prj.name}/vendor/glm/glm/**.inl",
			"%{prj.name}/vendor/stb_image/**.cpp",
			"%{prj.name}/vendor/stb_image/**.h",
		}
	
		includedirs
		{
			"%{prj.name}/src",
			"%{prj.name}/vendor/spdlog/include",
			"%{prj.name}/vendor/glm/",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Glad}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.stb_image}",
			"%{IncludeDir.entt}"
	
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
	
	
		filter "configurations:Debug"
			defines "OGLFRAMEWORK_DEBUG"
			symbols "On"
	
		filter "configurations:Release"
			defines "OGLFRAMEWORK_RELEASE"
			optimize "On"
	
