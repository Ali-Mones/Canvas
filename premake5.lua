workspace "OpenGL"
    configurations { "Debug", "Release" }
    language "C++"
    cppdialect "C++20"
    system "windows"
    architecture "x64"
    startproject "Sandbox"

    flags {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Canvas/vendor/msdf-atlas-gen"
group ""

    
project "Canvas"
    location "Canvas"
    kind "SharedLib"
    staticruntime "off"
    files { "%{prj.location}/**.hpp", "%{prj.location}/**.cpp", "%{prj.location}/**.h" }


    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    postbuildcommands {
        "{COPY} %{wks.location}bin\\".. outputdir .."\\%{prj.name}\\Canvas.dll %{wks.location}bin\\".. outputdir .."\\Sandbox\\"
    }

    defines {
        "BUILD_DLL",
        "GLEW_STATIC"
    }

    links { 
        "glfw3",
        "glfw3_mt",
        "glfw3dll",
        "opengl32",
        "glew32s",
        "msdf-atlas-gen"
    }

    libdirs {
        "%{prj.location}/vendor/GLFW/lib",
        "%{prj.location}/vendor/glew-2.1.0/lib/Release/x64",
    }

    includedirs { 
        "%{prj.location}/src",
        "%{prj.location}/vendor",
        "%{prj.location}/vendor/GLFW/include",
        "%{prj.location}/vendor/glew-2.1.0/include/GL",
        "%{prj.location}/vendor/msdf-atlas-gen/msdf-atlas-gen",
        "%{prj.location}/vendor/msdf-atlas-gen/msdfgen",
        "%{prj.location}/vendor/msdf-atlas-gen/msdfgen/include",
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
