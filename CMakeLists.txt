cmake_minimum_required(VERSION 3.0.0)
project(Renderize)

# useful variables
set(SourcePath ${PROJECT_SOURCE_DIR}/src)
set(ExtPath ${PROJECT_SOURCE_DIR}/ext)
set(LibPath ${PROJECT_SOURCE_DIR}/lib)

# include directories
include_directories(${SourcePath})
include_directories(${ExtPath})

# CMAKE flags
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/build)

# C++ compilation flags
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DGLEW_STATIC")

include(CMakeSrc.cmake)
add_executable(Renderize ${SOURCE_FILES_ALL})



### External projects ###
### GLFW ###
set(GLFWPath ${ExtPath}/glfw-3.1.1)
add_subdirectory(${GLFWPath})
include_directories(${GLFWPath}/include)
target_link_libraries(Renderize glfw ${GLFW_LIBRARIES})

### GLEW ###
set(GLEWPath ${ExtPath}/glew-1.13.0)
include_directories(${GLEWPath}/include)
target_link_libraries(Renderize debug ${GLEWPath}/lib/Debug/Win32/glew32sd.lib)
target_link_libraries(Renderize optimized ${GLEWPath}/lib/Release/Win32/glew32s.lib)

### SOIL ###
set(SOILPath ${ExtPath}/SOIL)
include_directories(${SOILPath})
target_link_libraries(Renderize debug ${SOILPath}/lib/Debug/SOIL.lib)
target_link_libraries(Renderize optimized ${SOILPath}/lib/Release/SOIL.lib)

### GLM ###
set(GLMPath ${ExtPath}/glm-0.9.7.1)
include_directories(${GLMPath})

### AntTweakBar ###
set(ATBPath ${ExtPath}/AntTweakBar)
include_directories(${ATBPath}/include)
target_link_libraries(Renderize ${ATBPath}/lib/AntTweakBar.lib)

### ASSIMP ###
#set(AssimpPath ${ExtPath}/assimp-3.1.1)
#include_directories(${AssimpPath}/include)
#target_link_libraries(Renderize ${AssimpPath}/lib/assimp.lib)



### Copying stuff ###
add_custom_command(TARGET Renderize PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_custom_command(TARGET Renderize PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)

add_custom_target(CopyShaderFiles)
foreach(ShaderFile ${SOURCE_FILES_SHADERS_SOURCE})
    get_filename_component(ShaderFileName ${ShaderFile} NAME)
    add_custom_command(TARGET CopyShaderFiles PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${ShaderFile} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${ShaderFileName})
endforeach()
add_dependencies(Renderize CopyShaderFiles)

add_custom_target(CopyDlls)
foreach(DllFile ${ATBPath}/lib/AntTweakBar.dll)
    get_filename_component(DllFileName ${DllFile} NAME)
    add_custom_command(TARGET CopyDlls PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DllFile} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${DllFileName})
endforeach()
add_dependencies(Renderize CopyDlls)

