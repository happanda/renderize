cmake_minimum_required(VERSION 2.8.10)
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
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/build/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/build/release)

# C++ compilation flags
# no flags :)

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

