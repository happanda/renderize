# C++ source files
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
source_group(shaders  FILES  ${SOURCE_FILES_SHADERS})


set(SOURCE_FILES_ALL
${SOURCE_FILES}
#${SOURCE_FILES_GLFW}
)

