# C++ source files
set(SOURCE_FILES_APP
${SourcePath}/app/app.h
${SourcePath}/app/app.cpp
)

set(SOURCE_FILES_BUFFERS
${SourcePath}/buffers/frame_buffer.h
${SourcePath}/buffers/frame_buffer.cpp
${SourcePath}/buffers/fwd.h
${SourcePath}/buffers/render_buffer.h
${SourcePath}/buffers/render_buffer.cpp
${SourcePath}/buffers/texture.h
${SourcePath}/buffers/texture.cpp
)

set(SOURCE_FILES_CAMERA
${SourcePath}/camera/camera.h
${SourcePath}/camera/camera.cpp
)

set(SOURCE_FILES_DATA
${SourcePath}/data/cube.h
${SourcePath}/data/cube.cpp
${SourcePath}/data/cycle.h
${SourcePath}/data/light.h
${SourcePath}/data/light.cpp
${SourcePath}/data/mesh.h
${SourcePath}/data/mesh.cpp
${SourcePath}/data/mesh_sorter.h
${SourcePath}/data/mesh_sorter.cpp
${SourcePath}/data/model.h
${SourcePath}/data/model.cpp
${SourcePath}/data/point.h
${SourcePath}/data/point.cpp
${SourcePath}/data/quad.h
${SourcePath}/data/quad.cpp
${SourcePath}/data/triangle.h
${SourcePath}/data/triangle.cpp
)

set(SOURCE_FILES_SHADERS
${SourcePath}/shaders/fwd.h
${SourcePath}/shaders/program.h
${SourcePath}/shaders/program.cpp
${SourcePath}/shaders/shader.h
${SourcePath}/shaders/shader.cpp
${SourcePath}/shaders/uniform.h
${SourcePath}/shaders/uniform.cpp
)

set(SOURCE_FILES_SHADERS_SOURCE
${SourcePath}/shaders_src/anaglyph.frag
${SourcePath}/shaders_src/asis.frag
${SourcePath}/shaders_src/asis.vert
${SourcePath}/shaders_src/clock.frag
${SourcePath}/shaders_src/common.frag
${SourcePath}/shaders_src/complex.frag
${SourcePath}/shaders_src/cube.vert
${SourcePath}/shaders_src/cube.frag
${SourcePath}/shaders_src/form.frag
${SourcePath}/shaders_src/fragment_demo.vert
${SourcePath}/shaders_src/lamp.frag
${SourcePath}/shaders_src/landscape.frag
${SourcePath}/shaders_src/mondria.frag
${SourcePath}/shaders_src/simple.vert
${SourcePath}/shaders_src/simple.frag
${SourcePath}/shaders_src/solid_color.frag
${SourcePath}/shaders_src/tex_noise.frag
${SourcePath}/shaders_src/universe.frag
${SourcePath}/shaders_src/visual.frag
)

set(SOURCE_FILES_UTIL
${SourcePath}/util/checked_call.h
${SourcePath}/util/date.h
${SourcePath}/util/date.cpp
${SourcePath}/util/ref_counter.h
${SourcePath}/util/ref_counter.cpp
${SourcePath}/util/soil_image.h
${SourcePath}/util/soil_image.cpp
)

set(SOURCE_FILES
#${SourcePath}/cubes.h
#${SourcePath}/cubes.cpp
${SourcePath}/main.cpp
)

#set(SOURCE_FILES_GLFW
#${ExtPath}/GLFW/glfw3.h
#${ExtPath}/GLFW/glfw3native.h
#)

# set appropriate source groups
#source_group(glfw  FILES  ${SOURCE_FILES_GLFW})
source_group(""  FILES  ${SOURCE_FILES})
source_group(app FILES ${SOURCE_FILES_APP})
source_group(buffers  FILES  ${SOURCE_FILES_BUFFERS})
source_group(camera FILES ${SOURCE_FILES_CAMERA})
source_group(data FILES ${SOURCE_FILES_DATA})
source_group(shaders  FILES  ${SOURCE_FILES_SHADERS})
source_group(shaders_src  FILES  ${SOURCE_FILES_SHADERS_SOURCE})
source_group(util  FILES  ${SOURCE_FILES_UTIL})
set_source_files_properties(${SOURCE_FILES_SHADERS_SOURCE} PROPERTIES HEADER_FILE_ONLY TRUE)


set(SOURCE_FILES_ALL
${SOURCE_FILES}
${SOURCE_FILES_UTIL}
${SOURCE_FILES_APP}
${SOURCE_FILES_BUFFERS}
${SOURCE_FILES_CAMERA}
${SOURCE_FILES_DATA}
${SOURCE_FILES_SHADERS}
${SOURCE_FILES_SHADERS_SOURCE}
#${SOURCE_FILES_GLFW}
)

