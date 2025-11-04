# CMake entry point
cmake_minimum_required (VERSION 4.0)
project (model_viewer)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

# Compile external dependencies 
add_subdirectory (external)
add_subdirectory (external/nativefiledialog-extended-1.2.1)

# On Visual 2005 and above, this module can set the debug working directory
# cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH 
"${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273"
# "${CMAKE_SOURCE_DIR}/external/nativefiledialog-extended-1.2.1"
)
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP


include_directories(
	external/glfw-3.4/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/imgui/
	external/imgui/backends/
	external/nativefiledialog-extended-1.2.1/src/include/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
	imgui
	nfd
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

file(GLOB_RECURSE MODEL_VIEWER_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/source/shaders/*.glsl"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/common/*.h"
)

add_executable(model_viewer ${MODEL_VIEWER_SOURCES})

target_link_libraries(model_viewer ${ALL_LIBS})

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
file(GLOB GLSL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/source/shaders/*.glsl")
foreach(GLSL_FILE ${GLSL_FILES})
    add_custom_command(
        TARGET model_viewer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GLSL_FILE}" "${CMAKE_CURRENT_BINARY_DIR}/"
        COMMENT "Copying ${GLSL_FILE} to build directory"
    )
endforeach()

elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

