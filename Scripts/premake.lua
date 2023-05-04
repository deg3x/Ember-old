
------------ Configuration Variables ------------

SOLUTION_NAME = "GraphicsPlayground"
SOLUTION_PATH = ".."
BIN_DIR = "../bin"
OBJ_DIR = "../obj"

ENGINE_PROJ_NAME = "Engine"
ENGINE_DIR = "../Engine"
ENGINE_BIN_DIR = BIN_DIR .. "/" .. ENGINE_PROJ_NAME
ENGINE_DEFINES = { "ENGINE_SHARED_LIB" }
ENGINE_LIBS_WIN = { "glfw3", "assimp-vc142-mt"}
ENGINE_LIBS_OSX = { "glfw3", "assimp.5.2.4", "IOKit.framework", "Cocoa.framework" }
ENGINE_INCL_PATH = { "../ThirdParty/includes/universal" }
ENGINE_INCL_PATH_WIN = { "../ThirdParty/includes/windows" }
ENGINE_INCL_PATH_OSX = { "../ThirdParty/includes/osx" }
ENGINE_LIBS_PATH_WIN = { "../ThirdParty/libraries/windows" }
ENGINE_LIBS_PATH_OSX = { "../ThirdParty/libraries/osx" }

EDITOR_PROJ_NAME = "Editor"
EDITOR_DIR = "../Editor"
EDITOR_BIN_DIR = BIN_DIR .. "/" .. EDITOR_PROJ_NAME
EDITOR_LIBS_WIN = { "Engine" }
EDITOR_LIBS_OSX = { "Engine" }
EDITOR_INCL_PATH = { ENGINE_DIR, "../ThirdParty/includes/universal" }
EDITOR_INCL_PATH_WIN = {}
EDITOR_INCL_PATH_OSX = {}
EDITOR_LIBS_PATH_WIN = { ENGINE_BIN_DIR .. "/Debug", ENGINE_BIN_DIR .. "/Release" }
EDITOR_LIBS_PATH_OSX = {}

-------------------------------------------------

if (_ACTION == "clean") then
    if os.target() == "windows" then
        os.execute("rmdir /s /q ..\\.vs")
        os.execute("rmdir /s /q ..\\.idea")
        os.execute("rmdir /s /q ..\\bin")
        os.execute("rmdir /s /q ..\\obj")
        os.execute("del ..\\" .. SOLUTION_NAME .. ".sln")
        os.execute("del ..\\*.vcxproj")
        os.execute("del ..\\*.vcxproj.user")
        os.execute("del ..\\*.vcxproj.filters")
    elseif os.target() == "macosx" then
        
    end
    os.exit()
end

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

---- ENGINE PROJECT ----
project (ENGINE_PROJ_NAME)
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    files 
    {
        ENGINE_DIR .. "/**.cpp",
        ENGINE_DIR .. "/**.c",
        ENGINE_DIR .. "/**.h",
        ENGINE_DIR .. "/**.glsl"
    }

    includedirs (ENGINE_INCL_PATH)
    defines (ENGINE_DEFINES)

    if os.target() == "windows" then
        links (ENGINE_LIBS_WIN)
        libdirs (ENGINE_LIBS_PATH_WIN)
        includedirs (ENGINE_INCL_PATH_WIN)
        -- removefiles (ENGINE_FILES_EXCLUDE_WIN)
    elseif os.target() == "macosx" then
        links (ENGINE_LIBS_OSX)
        libdirs (ENGINE_LIBS_PATH_OSX)
        includedirs (ENGINE_INCL_PATH_OSX)
        -- removefiles (ENGINE_FILES_EXCLUDE_OSX)
    end

    flags { "MultiProcessorCompile" }

    filter "configurations:Debug"
        targetdir (ENGINE_BIN_DIR .. "/Debug")
        defines { "DEBUG" }
        optimize "Off"
        symbols "On"

    filter "configurations:Release"
        targetdir (ENGINE_BIN_DIR .. "/Release")
        defines { "RELEASE" }
        optimize "Speed"
        symbols "Off"

---- EDITOR PROJECT ----
project (EDITOR_PROJ_NAME)
    kind "ConsoleApp"
    dependson { ENGINE_PROJ_NAME }
    language "C++"
    cppdialect "C++20"

    files 
    {
        EDITOR_DIR .. "/**.cpp",
        EDITOR_DIR .. "/**.c",
        EDITOR_DIR .. "/**.h",
    }

    includedirs (EDITOR_INCL_PATH)

    if os.target() == "windows" then
        links (EDITOR_LIBS_WIN)
        libdirs (EDITOR_LIBS_PATH_WIN)
        includedirs (EDITOR_INCL_PATH_WIN)

        postbuildcommands
        {
            "{COPY} %{wks.location}/ThirdParty/libraries/windows/assimp-vc142-mt.dll %{cfg.targetdir}" 
        }

        -- removefiles (ENGINE_FILES_EXCLUDE_WIN)
    elseif os.target() == "macosx" then
        links (EDITOR_LIBS_OSX)
        libdirs (EDITOR_LIBS_PATH_OSX)
        includedirs (EDITOR_INCL_PATH_OSX)

        postbuildcommands
        {
            "{COPY} %{wks.location}/ThirdParty/libraries/osx/libassimp.5.2.4.dylib %{cfg.targetdir}",
            "{COPY} %{wks.location}/ThirdParty/libraries/osx/libassimp.5.dylib %{cfg.targetdir}",
            "{COPY} %{wks.location}/ThirdParty/libraries/osx/libassimp.dylib %{cfg.targetdir}" 
        }

        -- removefiles (ENGINE_FILES_EXCLUDE_OSX)
    end

    filter "configurations:Debug"
        targetdir (EDITOR_BIN_DIR .. "/Debug")
        defines { "DEBUG" }
        optimize "Off"
        symbols "On"

        if os.target() == "windows" then
            postbuildcommands
            {
                "{COPY} %{wks.location}/bin/" .. ENGINE_PROJ_NAME .. "/Debug/" .. ENGINE_PROJ_NAME .. ".dll %{cfg.targetdir}"
            }
        elseif os.target() == "macosx" then
            postbuildcommands
            {

            }
        end

    filter "configurations:Release"
        targetdir (EDITOR_BIN_DIR .. "/Release")
        defines { "RELEASE" }
        optimize "Speed"
        symbols "Off"

        if os.target() == "windows" then
            postbuildcommands
            {
                "{COPY} %{wks.location}/bin/" .. ENGINE_PROJ_NAME .. "/Release/" .. ENGINE_PROJ_NAME .. ".dll %{cfg.targetdir}"
            }
        elseif os.target() == "macosx" then
            postbuildcommands
            {
                
            }
        end
