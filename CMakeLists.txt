cmake_minimum_required(VERSION 3.20)

# -------- Configuration Variables --------
set(PROJECT_NAME "Ember")
set(PROJECT_PATH "${CMAKE_SOURCE_DIR}")
set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin")
set(OBJ_DIR "${CMAKE_SOURCE_DIR}/obj")

set(ENGINE_TARGET_NAME "Engine")
set(ENGINE_DIR "${CMAKE_SOURCE_DIR}/Engine")
set(ENGINE_BIN_DIR "${BIN_DIR}/${ENGINE_TARGET_NAME}")
set(ENGINE_DEFINES "ENGINE_SHARED_LIB")
set(ENGINE_LIBS_WIN "glfw3dll" "assimp-vc142-mt")
set(ENGINE_LIBS_OSX "glfw.3" "assimp.5.2.4" "IOKit.framework" "Cocoa.framework")
set(ENGINE_INCL_PATH "${ENGINE_DIR}" "${CMAKE_SOURCE_DIR}/ThirdParty/includes/universal")
set(ENGINE_INCL_PATH_WIN "${CMAKE_SOURCE_DIR}/ThirdParty/includes/windows")
set(ENGINE_INCL_PATH_OSX "${CMAKE_SOURCE_DIR}/ThirdParty/includes/osx")
set(ENGINE_LIBS_PATH_WIN "${CMAKE_SOURCE_DIR}/ThirdParty/libraries/windows")
set(ENGINE_LIBS_PATH_OSX "${CMAKE_SOURCE_DIR}/ThirdParty/libraries/osx")

set(EDITOR_TARGET_NAME "Editor")
set(EDITOR_DIR "${CMAKE_SOURCE_DIR}/Editor")
set(EDITOR_BIN_DIR "${BIN_DIR}/${EDITOR_TARGET_NAME}")
set(EDITOR_LIBS_WIN "glfw3dll")
set(EDITOR_LIBS_OSX "glfw.3")
set(EDITOR_INCL_PATH "${ENGINE_DIR}" "${EDITOR_DIR}" "${CMAKE_SOURCE_DIR}/ThirdParty/includes/universal")
set(EDITOR_INCL_PATH_WIN "${CMAKE_SOURCE_DIR}/ThirdParty/includes/windows")
set(EDITOR_INCL_PATH_OSX "${CMAKE_SOURCE_DIR}/ThirdParty/includes/osx")
set(EDITOR_LIBS_PATH_WIN "${CMAKE_SOURCE_DIR}/ThirdParty/libraries/windows")
set(EDITOR_LIBS_PATH_OSX "${CMAKE_SOURCE_DIR}/ThirdParty/libraries/osx")

file(GLOB_RECURSE ENGINE_CPP_FILES "${ENGINE_DIR}/*.cpp")
file(GLOB_RECURSE ENGINE_C_FILES "${ENGINE_DIR}/*.c")
set(ENGINE_SRC_FILES ${ENGINE_CPP_FILES} ${ENGINE_C_FILES})

file(GLOB_RECURSE EDITOR_CPP_FILES "${EDITOR_DIR}/*.cpp")
set(EDITOR_SRC_FILES ${EDITOR_CPP_FILES})

if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

set(CMAKE_CXX_STANDARD 20)
project(${PROJECT_NAME} CXX)
set_source_files_properties(${ENGINE_C_FILES} PROPERTIES LANGUAGE CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OBJ_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR})

# -------- ENGINE --------
add_library(${ENGINE_TARGET_NAME} SHARED ${ENGINE_SRC_FILES})

target_compile_features(${ENGINE_TARGET_NAME} PRIVATE cxx_std_20)
target_include_directories(${ENGINE_TARGET_NAME} PRIVATE ${ENGINE_INCL_PATH})
target_compile_definitions(${ENGINE_TARGET_NAME} PRIVATE ${ENGINE_DEFINES})
target_precompile_headers(${ENGINE_TARGET_NAME} PRIVATE ${ENGINE_DIR}/engine_pch.h)

if (WIN32)
    target_link_libraries(${ENGINE_TARGET_NAME} PRIVATE ${ENGINE_LIBS_WIN})
    target_include_directories(${ENGINE_TARGET_NAME} PRIVATE ${ENGINE_INCL_PATH_WIN})
    target_link_directories(${ENGINE_TARGET_NAME} PRIVATE ${ENGINE_LIBS_PATH_WIN})
    
    add_custom_command(
        TARGET ${ENGINE_TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${ENGINE_LIBS_PATH_WIN}/glfw3.dll $<TARGET_FILE_DIR:${ENGINE_TARGET_NAME}>
    )
    
elseif (APPLE)
    target_link_libraries(${ENGINE_TARGET_NAME} PRIVATE ${ENGINE_LIBS_OSX})
    target_include_directories(${ENGINE_TARGET_NAME} PRIVATE ${ENGINE_INCL_PATH_OSX})
    target_link_directories(${ENGINE_TARGET_NAME} PRIVATE ${ENGINE_LIBS_PATH_OSX})
    
    add_custom_command(
        TARGET ${ENGINE_TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${ENGINE_LIBS_PATH_OSX}/libglfw.3.dylib $<TARGET_FILE_DIR:${ENGINE_TARGET_NAME}>
    )
endif()

set_target_properties(
    ${ENGINE_TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${ENGINE_BIN_DIR}/Debug
)

set_target_properties(
    ${ENGINE_TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${ENGINE_BIN_DIR}/Release
)

# -------- EDITOR --------
add_executable(${EDITOR_TARGET_NAME} ${EDITOR_SRC_FILES})

target_compile_features(${EDITOR_TARGET_NAME} PRIVATE cxx_std_20)
target_include_directories(${EDITOR_TARGET_NAME} PRIVATE ${EDITOR_INCL_PATH})
target_precompile_headers(${EDITOR_TARGET_NAME} PRIVATE ${EDITOR_DIR}/editor_pch.h)

target_link_libraries(${EDITOR_TARGET_NAME} PRIVATE ${ENGINE_TARGET_NAME})

if (WIN32)
    target_link_libraries(${EDITOR_TARGET_NAME} PRIVATE ${EDITOR_LIBS_WIN})
    target_include_directories(${EDITOR_TARGET_NAME} PRIVATE ${EDITOR_INCL_PATH_WIN})
    target_link_directories(${EDITOR_TARGET_NAME} PRIVATE ${EDITOR_LIBS_PATH_WIN})
    
    add_custom_command(
        TARGET ${EDITOR_TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${EDITOR_LIBS_PATH_WIN}/glfw3.dll $<TARGET_FILE_DIR:${EDITOR_TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy ${EDITOR_LIBS_PATH_WIN}/assimp-vc142-mt.dll $<TARGET_FILE_DIR:${EDITOR_TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${ENGINE_TARGET_NAME}>/Engine.dll $<TARGET_FILE_DIR:${EDITOR_TARGET_NAME}>
    )
elseif (APPLE)
    target_link_libraries(${EDITOR_TARGET_NAME} PRIVATE ${EDITOR_LIBS_OSX})
    target_include_directories(${EDITOR_TARGET_NAME} PRIVATE ${EDITOR_INCL_PATH_OSX})
    target_link_directories(${EDITOR_TARGET_NAME} PRIVATE ${EDITOR_LIBS_PATH_OSX})
    
    add_custom_command(
        TARGET ${EDITOR_TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${EDITOR_LIBS_PATH_OSX}/libglfw.3.dylib $<TARGET_FILE_DIR:${EDITOR_TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy ${EDITOR_LIBS_PATH_OSX}/libassimp.5.2.4.dylib $<TARGET_FILE_DIR:${EDITOR_TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${ENGINE_TARGET_NAME}>/Engine.dylib $<TARGET_FILE_DIR:${EDITOR_TARGET_NAME}>
    )
endif()

set_target_properties(
    ${EDITOR_TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EDITOR_BIN_DIR}/Debug
)

set_target_properties(
    ${EDITOR_TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EDITOR_BIN_DIR}/Release
)
