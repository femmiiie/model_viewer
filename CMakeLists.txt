# CMake entry point
cmake_minimum_required (VERSION 4.0)
project (model_viewer)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
# cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP


include_directories(
	external/glfw-3.4/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/imgui/
	external/imgui/backends
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
	imgui
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)


add_executable(model_viewer
	source/main.cpp
	source/Object/MeshObject.cpp
	source/Object/MeshObject.h
	source/Object/GridObject.cpp
	source/Object/GridObject.h
	source/Object/AxesObject.cpp
	source/Object/AxesObject.h
	source/Object/Object.cpp
	source/Object/Object.h
	source/Renderer/Renderer.cpp
	source/Renderer/Renderer.h
	source/Camera/Camera.cpp
	source/Camera/Camera.h
	common/shader.cpp
	common/shader.hpp
	common/controls.cpp
	common/controls.hpp
	common/texture.cpp
	common/texture.hpp
	common/objloader.cpp
	common/objloader.hpp
	common/vboindexer.cpp
	common/vboindexer.hpp
	
	source/mesh.vs.glsl
	source/mesh.fs.glsl
	source/grid.vs.glsl
	source/grid.fs.glsl
	source/axes.vs.glsl
	source/axes.gs.glsl
)
target_link_libraries(model_viewer
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(model_viewer PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source/")
create_target_launcher(model_viewer WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/source/")



SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
   TARGET model_viewer POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/model_viewer${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/source/"
)

# Copy all .glsl files to build directory
file(GLOB GLSL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.glsl")
foreach(GLSL_FILE ${GLSL_FILES})
    add_custom_command(
        TARGET model_viewer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GLSL_FILE}" "${CMAKE_CURRENT_BINARY_DIR}/"
        COMMENT "Copying ${GLSL_FILE} to build directory"
    )
endforeach()

elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

