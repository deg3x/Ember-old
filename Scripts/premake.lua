
------------ Configuration Variables ------------

SOLUTION_NAME = "GraphicsPlayground"
SOLUTION_PATH = ".."
BIN_DIR = "../bin"
OBJ_DIR = "../obj"

ENGINE_PROJ_NAME = "Engine"
ENGINE_DIR = "../Engine"
ENGINE_LIBS = { "glfw3.lib" }
ENGINE_LIBS_PATH = { "../ThirdParty/libraries" }
ENGINE_INCL_PATH = { "../ThirdParty/includes" }

-------------------------------------------------

workspace (SOLUTION_NAME)
    location (SOLUTION_PATH)
    configurations { "Debug", "Release" }
    systemversion "latest"
    architecture "x64"
    platforms { "x64" }

project (ENGINE_PROJ_NAME)
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    files 
    {
        ENGINE_DIR .. "/**.cpp",
        ENGINE_DIR .. "/**.c",
        ENGINE_DIR .. "/**.h",
        ENGINE_DIR .. "/**.shader"
    }

    links (ENGINE_LIBS)

    libdirs (ENGINE_LIBS_PATH)

    includedirs (ENGINE_INCL_PATH)

    flags { "MultiProcessorCompile" }

    filter "configurations:Debug"
        targetdir (BIN_DIR .. "/Debug")
        defines { "DEBUG" }
        optimize "Off"
        symbols "On"

    filter "configurations:Release"
        targetdir (BIN_DIR .. "/Release")
        defines { "RELEASE" }
        optimize "Speed"
        symbols "Off"