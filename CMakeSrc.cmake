# C++ source files
set(SOURCE_FILES_CAMERA
${SourcePath}/camera/camera.h
${SourcePath}/camera/camera.cpp
)

set(SOURCE_FILES_SHADERS
${SourcePath}/shaders/program.h
${SourcePath}/shaders/program.cpp
${SourcePath}/shaders/shader.h
${SourcePath}/shaders/shader.cpp
)

set(SOURCE_FILES
${SOURCE_FILES_SHADERS}
${SourcePath}/main.cpp
)

#set(SOURCE_FILES_GLFW
#${ExtPath}/GLFW/glfw3.h
#${ExtPath}/GLFW/glfw3native.h
#)

# set appropriate source groups
#source_group(glfw  FILES  ${SOURCE_FILES_GLFW})
source_group(""  FILES  ${SourcePath}/main.cpp)
source_group(camera FILES ${SOURCE_FILES_CAMERA})
source_group(shaders  FILES  ${SOURCE_FILES_SHADERS})


set(SOURCE_FILES_ALL
${SOURCE_FILES}
${SOURCE_FILES_CAMERA}
${SOURCE_FILES_SHADERS}
#${SOURCE_FILES_GLFW}
)

