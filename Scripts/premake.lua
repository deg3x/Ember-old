
------------ Configuration Variables ------------

SOLUTION_NAME = "GraphicsPlayground"
SOLUTION_PATH = ".."
BIN_DIR = "../bin"
OBJ_DIR = "../obj"

ENGINE_PROJ_NAME = "Engine"
ENGINE_DIR = "../Engine"
ENGINE_LIBS_WIN = { "glfw3", "assimp"}
ENGINE_LIBS_OSX = { "glfw3", "IOKit.framework", "Cocoa.framework" }
ENGINE_INCL_PATH = { "../ThirdParty/includes/universal" }
ENGINE_INCL_PATH_WIN = { "../ThirdParty/includes/windows" }
ENGINE_INCL_PATH_OSX = { "../ThirdParty/includes/osx" }
ENGINE_LIBS_PATH_WIN = { "../ThirdParty/libraries/windows" }
ENGINE_LIBS_PATH_OSX = { "../ThirdParty/libraries/osx" }
-- ENGINE_FILES_EXCLUDE_WIN = { ENGINE_DIR .. "/core/glad_osx.c" }
-- ENGINE_FILES_EXCLUDE_OSX = { ENGINE_DIR .. "/core/glad.c" }

-------------------------------------------------

workspace (SOLUTION_NAME)
    location (SOLUTION_PATH)
    configurations { "Debug", "Release" }
    systemversion "latest"

    if os.target() == "windows" then
        architecture "x64"
        platforms { "win-x64" }
    elseif os.target() == "macosx" then
        architecture "ARM64"
        platforms { "osx-x64" }
    end

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

    includedirs (ENGINE_INCL_PATH)

    if os.target() == "windows" then
        links (ENGINE_LIBS_WIN)
        libdirs (ENGINE_LIBS_PATH_WIN)
        includedirs (ENGINE_INCL_PATH_WIN)
        
        postbuildcommands
        {
            "{COPY} %{wks.location}/ThirdParty/libraries/windows/assimp-vc142-mt.dll %{cfg.targetdir}" 
        }
        -- removefiles (ENGINE_FILES_EXCLUDE_WIN)
    elseif os.target() == "macosx" then
        links (ENGINE_LIBS_OSX)
        libdirs (ENGINE_LIBS_PATH_OSX)
        includedirs (ENGINE_INCL_PATH_OSX)
        -- removefiles (ENGINE_FILES_EXCLUDE_OSX)
    end

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