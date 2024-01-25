
------------ Configuration Variables ------------

SOLUTION_NAME = "GraphicsPlayground"
SOLUTION_PATH = ".."
BIN_DIR = "../bin"
OBJ_DIR = "../obj"

ENGINE_PROJ_NAME = "Engine"
ENGINE_DIR = "../Engine"
ENGINE_BIN_DIR = BIN_DIR .. "/" .. ENGINE_PROJ_NAME
ENGINE_DEFINES = { "ENGINE_SHARED_LIB" }
ENGINE_LIBS_WIN = { "glfw3dll", "assimp-vc142-mt"}
ENGINE_LIBS_OSX = { "glfw.3", "assimp.5.2.4", "IOKit.framework", "Cocoa.framework" }
ENGINE_INCL_PATH = { ENGINE_DIR, "../ThirdParty/includes/universal" }
ENGINE_INCL_PATH_WIN = { "../ThirdParty/includes/windows" }
ENGINE_INCL_PATH_OSX = { "../ThirdParty/includes/osx" }
ENGINE_LIBS_PATH_WIN = { "../ThirdParty/libraries/windows" }
ENGINE_LIBS_PATH_OSX = { "../ThirdParty/libraries/osx" }

EDITOR_PROJ_NAME = "Editor"
EDITOR_DIR = "../Editor"
EDITOR_BIN_DIR = BIN_DIR .. "/" .. EDITOR_PROJ_NAME
EDITOR_LIBS_WIN = { "Engine", "glfw3dll" }
EDITOR_LIBS_OSX = { "Engine", "glfw.3" }
EDITOR_INCL_PATH = { ENGINE_DIR, EDITOR_DIR, "../ThirdParty/includes/universal" }
EDITOR_INCL_PATH_WIN = { "../ThirdParty/includes/windows" }
EDITOR_INCL_PATH_OSX = { "../ThirdParty/includes/osx" }
EDITOR_LIBS_PATH_WIN = { "../ThirdParty/libraries/windows", ENGINE_BIN_DIR .. "/Debug", ENGINE_BIN_DIR .. "/Release" }
EDITOR_LIBS_PATH_OSX = { "../ThirdParty/libraries/osx" }

CLEAN_DIRS_WIN = { ".vs", ".idea", "bin", "obj" }
CLEAN_FILES_WIN = { SOLUTION_NAME .. ".sln", "*.vcxproj*" }
CLEAN_DIRS_OSX = { ".vs", ".idea", "bin", "obj", SOLUTION_NAME .. ".xcworkspace", ENGINE_PROJ_NAME .. ".xcodeproj", EDITOR_PROJ_NAME .. ".xcodeproj" }
CLEAN_FILES_OSX = {}

-------------------------------------------------

if (_ACTION == "clean") then
    if os.target() == "windows" then
        for index, dir in ipairs(CLEAN_DIRS_WIN) do
            os.execute("if exist ..\\" .. dir .. " (rmdir /s /q ..\\" .. dir .. " && echo Deleted directory " .. dir .. ")")
        end
        for index, file in ipairs(CLEAN_FILES_WIN) do
            os.execute("if exist ..\\" .. file .. " (del ..\\" .. file .. " && echo Deleted file " .. file .. ")")
        end
    elseif os.target() == "macosx" then
        for index, dir in ipairs(CLEAN_DIRS_OSX) do
            os.execute("if test -d \"../" .. dir .. "\"; then (rm -r ../" .. dir .. " && echo Deleted directory " .. dir .. ") fi")
        end
        for index, file in ipairs(CLEAN_FILES_OSX) do
            os.execute("if test -f \"../" .. file .. "\"; then (rm ../" .. file .. " && echo Deleted file " .. file .. ") fi")
        end
    end
    os.exit()
end

workspace (SOLUTION_NAME)
    location (SOLUTION_PATH)
    configurations { "Debug", "Release" }
    systemversion "latest"
    startproject (EDITOR_PROJ_NAME)

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

    pchheader "engine_pch.h"
    pchsource (ENGINE_DIR .. "/engine_pch.cpp")

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

        postbuildcommands
        {
            "{COPY} %{wks.location}/ThirdParty/libraries/windows/glfw3.dll %{cfg.targetdir}"
        }

    elseif os.target() == "macosx" then
        links (ENGINE_LIBS_OSX)
        libdirs (ENGINE_LIBS_PATH_OSX)
        includedirs (ENGINE_INCL_PATH_OSX)
        -- removefiles (ENGINE_FILES_EXCLUDE_OSX)

        postbuildcommands
        {
            "{COPY} %{wks.location}/ThirdParty/libraries/osx/libglfw.3.dylib %{cfg.targetdir}"
        }

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

    filter "files:**.c"
        flags { "NoPCH" }

---- EDITOR PROJECT ----
project (EDITOR_PROJ_NAME)
    kind "ConsoleApp"
    dependson { ENGINE_PROJ_NAME }
    language "C++"
    cppdialect "C++20"

    pchheader "editor_pch.h"
    pchsource (EDITOR_DIR .. "/editor_pch.cpp")

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
            "{COPY} %{wks.location}/ThirdParty/libraries/windows/assimp-vc142-mt.dll %{cfg.targetdir}",
            "{COPY} %{wks.location}/ThirdParty/libraries/windows/glfw3.dll %{cfg.targetdir}"
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
            "{COPY} %{wks.location}/ThirdParty/libraries/osx/libassimp.dylib %{cfg.targetdir}",
            "{COPY} %{wks.location}/ThirdParty/libraries/osx/libglfw.3.dylib %{cfg.targetdir}"
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
            --postbuildcommands
            --{

            --}
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
            --postbuildcommands
            --{
                
            --}
        end
    
    filter ("files:" .. EDITOR_DIR .. "/imgui/*.cpp")
        flags { "NoPCH" }
