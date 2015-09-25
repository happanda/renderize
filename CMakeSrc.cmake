# C++ source files
set(SOURCE_FILES
${SourcePath}/shaders/shader.h
${SourcePath}/shaders/shader.cpp
${SourcePath}/main.cpp
)

#set(SOURCE_FILES_GLFW
#${ExtPath}/GLFW/glfw3.h
#${ExtPath}/GLFW/glfw3native.h
#)

# set appropriate source groups
#source_group(glfw  FILES  ${SOURCE_FILES_GLFW})
source_group(""  FILES  ${SourcePath}/main.cpp)
source_group(shaders  FILES  ${SourcePath}/shaders/shader.h)
source_group(shaders  FILES  ${SourcePath}/shaders/shader.cpp)


set(SOURCE_FILES_ALL
${SOURCE_FILES}
#${SOURCE_FILES_GLFW}
)

