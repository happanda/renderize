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
${SourcePath}/shaders/uniform.h
${SourcePath}/shaders/uniform.cpp
)

set(SOURCE_FILES_TEXTURES
${SourcePath}/textures/texture.h
${SourcePath}/textures/texture.cpp
)

set(SOURCE_FILES_SHADERS_SOURCE
${SourcePath}/shaders/cube.vert
${SourcePath}/shaders/cube.frag
${SourcePath}/shaders/lamp.frag
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
source_group(textures  FILES  ${SOURCE_FILES_TEXTURES})
source_group(shaders_src  FILES  ${SOURCE_FILES_SHADERS_SOURCE})
set_source_files_properties(${SOURCE_FILES_SHADERS_SOURCE} PROPERTIES HEADER_FILE_ONLY TRUE)


set(SOURCE_FILES_ALL
${SOURCE_FILES}
${SOURCE_FILES_CAMERA}
${SOURCE_FILES_SHADERS}
${SOURCE_FILES_TEXTURES}
${SOURCE_FILES_SHADERS_SOURCE}
#${SOURCE_FILES_GLFW}
)

