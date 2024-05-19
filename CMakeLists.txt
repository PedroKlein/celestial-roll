cmake_minimum_required(VERSION 3.5.0)

project(CelestialRoll)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(EXECUTABLE_NAME celestial_roll)

if(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DRELEASE_BUILD)
endif()

add_executable(${EXECUTABLE_NAME}
    src/glad.c
    src/component.cpp
    src/componentType.cpp
    src/main.cpp
)

if(WIN32)
    if(MINGW)
        set(LIBGLFW ${PROJECT_SOURCE_DIR}/libs/lib/libglfw3.a)
    elseif(MSVC)
        set(LIBGLFW ${PROJECT_SOURCE_DIR}/libs/lib-vc2022/glfw3.lib)
    else()
        message(FATAL_ERROR "This CMakeLists.txt file only supports MINGW or MSVC toolchain on Windows.")
    endif()

    target_link_libraries(${EXECUTABLE_NAME} ${LIBGLFW} gdi32 opengl32)
  

elseif(APPLE) 
    find_library(GLFW_LIBRARY NAMES glfw3 HINTS "/usr/local/lib")

    target_link_libraries(${EXECUTABLE_NAME} "-framework OpenGL -framework CoreFoundation -framework CoreGraphics -framework IOKit -framework Cocoa")
    target_link_libraries(${EXECUTABLE_NAME} ${GLFW_LIBRARY})
    target_link_libraries(${EXECUTABLE_NAME} "-lm -ldl -lpthread")

elseif(UNIX)
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    find_library(MATH_LIBRARY m)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wno-unused-function)
    target_link_libraries(${EXECUTABLE_NAME}
        ${CMAKE_DL_LIBS}
        ${MATH_LIBRARY}
        ${PROJECT_SOURCE_DIR}/libs/lib-linux/libglfw3.a
        ${CMAKE_THREAD_LIBS_INIT}
        ${OPENGL_LIBRARIES}
        ${X11_LIBRARIES}
        ${X11_Xrandr_LIB}
        ${X11_Xcursor_LIB}
        ${X11_Xinerama_LIB}
        ${X11_Xxf86vm_LIB}
    )

endif()

target_include_directories(${EXECUTABLE_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/external)

set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINKER_LANGUAGE CXX)

file(GLOB SHADER_FILES ${CMAKE_SOURCE_DIR}/src/shaders/*)
file(GLOB MODEL_FILES ${CMAKE_SOURCE_DIR}/models/*)

add_custom_target(CopyModels ALL
                  COMMAND ${CMAKE_COMMAND} -E remove_directory
                  ${CMAKE_CURRENT_BINARY_DIR}/models
                  COMMAND ${CMAKE_COMMAND} -E copy_directory
                  ${CMAKE_SOURCE_DIR}/models ${CMAKE_CURRENT_BINARY_DIR}/models
                  DEPENDS ${MODEL_FILES})
add_custom_target(CopyShaders ALL
                  COMMAND ${CMAKE_COMMAND} -E remove_directory
                  ${CMAKE_CURRENT_BINARY_DIR}/shaders
                  COMMAND ${CMAKE_COMMAND} -E copy_directory
                  ${CMAKE_SOURCE_DIR}/src/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
                  DEPENDS ${SHADER_FILES})

add_dependencies(${EXECUTABLE_NAME} CopyShaders)
add_dependencies(${EXECUTABLE_NAME} CopyModels)