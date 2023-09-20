workspace "OpenGL"
    configurations { "Debug", "Release" }
    language "C++"
    cppdialect "C++20"
    architecture "x64"
    startproject "Sandbox"

    flags {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Canvas/vendor/msdf-atlas-gen"
    include "Canvas/vendor/glfw"
    include "Canvas/vendor/glad"
group ""

project "Canvas"
    location "Canvas"
    kind "SharedLib"
    staticruntime "off"
    files {
        "%{prj.location}/src/*.hpp",
        "%{prj.location}/src/*.cpp",
        "%{prj.location}/src/*.h",

        "%{prj.location}/vendor/glm/**.hpp",
        "%{prj.location}/vendor/glm/**.cpp",
        "%{prj.location}/vendor/glm/**.h",

        "%{prj.location}/vendor/imgui/**.hpp",
        "%{prj.location}/vendor/imgui/**.cpp",
        "%{prj.location}/vendor/imgui/**.h",

        "%{prj.location}/vendor/stb_image/**.hpp",
        "%{prj.location}/vendor/stb_image/**.cpp",
        "%{prj.location}/vendor/stb_image/**.h",

        "%{prj.location}/vendor/stb_image_write/**.hpp",
        "%{prj.location}/vendor/stb_image_write/**.cpp",
        "%{prj.location}/vendor/stb_image_write/**.h",

        "%{prj.location}/vendor/miniaudio/**.hpp",
        "%{prj.location}/vendor/miniaudio/**.cpp",
        "%{prj.location}/vendor/miniaudio/**.h",
    }


    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    defines {
        "BUILD_DLL",
    }

    links { 
        "glfw",
        "msdf-atlas-gen",
        "glad"
    }

    includedirs { 
        "%{prj.location}/src",
        "%{prj.location}/vendor",
        "%{prj.location}/vendor/glfw/include/GLFW",
        "%{prj.location}/vendor/glad/include",
        "%{prj.location}/vendor/msdf-atlas-gen/msdf-atlas-gen",
        "%{prj.location}/vendor/msdf-atlas-gen/msdfgen",
        "%{prj.location}/vendor/msdf-atlas-gen/msdfgen/include",
    }

    filter "system:windows"
        postbuildcommands {
            "{COPY} %{wks.location}bin\\".. outputdir .."\\%{prj.name}\\Canvas.dll %{wks.location}bin\\".. outputdir .."\\Sandbox\\"
        }

        links {
            "opengl32",
        }

    filter "system:linux"
        --postbuildcommands {
        --    "{COPY} %{wks.location}bin\\".. outputdir .."\\%{prj.name}\\libCanvas.so %{wks.location}bin\\".. outputdir .."\\Sandbox\\"
        --}

        links {
            "GL",
            "pthread",
            "m",
            "dl",
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    files { "%{prj.location}/**.hpp", "%{prj.location}/**.cpp", "%{prj.location}/**.h" }

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    links {
        "Canvas"
    }

    libdirs {
    }

    includedirs {
        "%{prj.location}/src",
        "%{wks.location}/Canvas/src",
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
