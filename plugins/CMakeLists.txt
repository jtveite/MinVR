
set(VRPlugins_checkout_dir ${PROJECT_SOURCE_DIR}/plugins)
set(VRPlugins_build_dir ${CMAKE_BINARY_DIR}/plugins)
set(VRPlugins_stamp_dir ${VRPlugins_build_dir}/stamp)
set(VRPlugins_tmp_dir ${VRPlugins_build_dir}/tmp)
make_directory(${VRPlugins_checkout_dir})
set(VRPlugins_install_dir "${CMAKE_INSTALL_PREFIX}/plugins")
set(VRPlugins_CMAKE_ARGS -DMinVR_INCLUDE_DIRS=${PROJECT_SOURCE_DIR}/.. -DMinVR_BINARY_DIRS=${CMAKE_BINARY_DIR}/lib -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(MinVRExtensions_CMAKE_ARGS ${VRPlugins_CMAKE_ARGS} -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD="c++11" -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY="libc++")
endif()

include(ExternalProject)

option(BASE_PLUGINS "If enabled, Base plugins will be installed" ON)
option(BASE_GRAPHICS_PLUGINS "If enabled, Base graphics plugins will be installed" OFF)
option(BASE_VR_PLUGINS "If enabled, Base virtual reality plugins will be installed" OFF)

if (BASE_VR_PLUGINS)
	set(BASE_PLUGINS ON)
endif(BASE_VR_PLUGINS)

if (BASE_PLUGINS)
	set(BASE_GRAPHICS_PLUGINS ON)
endif(BASE_PLUGINS)

#------------------OpenGL-----------------------------
option(MINVR_OPENGL_PLUGIN "If enabled, OpenGL plugin will be installed" OFF)
if (MINVR_OPENGL_PLUGIN OR BASE_GRAPHICS_PLUGINS)
	add_subdirectory (OpenGL)
endif()

#------------------GLFW-----------------------------
option(MINVR_GLFW_PLUGIN "If enabled, GLFW plugin will be installed" OFF)
if (MINVR_GLFW_PLUGIN OR BASE_GRAPHICS_PLUGINS)
	add_subdirectory (GLFW)
endif()

#------------------VRPN-----------------------------
option(MINVR_VRPN_PLUGIN "If enabled, VRPN plugin will be installed" OFF)
if (MINVR_VRPN_PLUGIN OR BASE_PLUGINS)
	add_subdirectory (VRPN)
endif()

#------------------Python-----------------------------
option(MINVR_PYTHON_PLUGIN "If enabled, Python plugin will be installed" OFF)
if (MINVR_PYTHON_PLUGIN OR BASE_PLUGINS)
	add_subdirectory (Python)
endif()

#------------------Threading-----------------------------
option(MINVR_THREADING_PLUGIN "If enabled, Threading plugin will be installed" OFF)
if (MINVR_THREADING_PLUGIN OR BASE_PLUGINS)
	add_subdirectory (Threading)
endif()

#------------------OpenVR-----------------------------
option(MINVR_OPENVR_PLUGIN "If enabled, OpenVR plugin will be installed" OFF)
if (MINVR_OPENVR_PLUGIN)
	add_subdirectory (OpenVR)
endif()

#------------------FreeGlut-----------------------------
option(MINVR_FREEGLUT_PLUGIN "If enabled, FreeGlut plugin will be installed" OFF)
if (MINVR_FREEGLUT_PLUGIN)
	add_subdirectory (FREEGLUT)
endif()

#------------------Scalable-----------------------------
option(MINVR_SCALABLE_PLUGIN "If enabled, SCALABLE plugin will be installed. Requires proprietary software from scalabledisplay.com" OFF)
if (MINVR_SCALABLE_PLUGIN)
	add_subdirectory (Scalable)
endif()



if(false)

#------------------Threading-----------------------------
option(MINVR_THREADING_PLUGIN "If enabled, Threading plugin will be installed" OFF)
if (MINVR_THREADING_PLUGIN OR BASE_GRAPHICS_PLUGINS)
	add_subdirectory (plugins/Threading)
endif()

#------------------GLUT-----------------------------
option(MINVR_GLUT_PLUGIN "If enabled, GLUT plugin will be installed" OFF)

find_package(GLUT)
if (GLUT_FOUND)
	if (MINVR_GLUT_PLUGIN OR BASE_GRAPHICS_PLUGINS)
		add_subdirectory (plugins/GLUT)
	endif()
endif()

endif()



#------------------GLFW-------------------------------

if (false)
	set(MinVRExtensions_checkout_Dir ${PROJECT_SOURCE_DIR}/plugins/MinVRExtensions)

	set(plugin_project_name MinVR_GLFW)
	set(MinVR_GLFW_prefix "${VRPlugins_checkout_dir}/${plugin_project_name}")
	make_directory(${MinVR_GLFW_prefix})
	set(project_CMAKE_ARGS ${VRPlugins_CMAKE_ARGS} -DCMAKE_INSTALL_PREFIX=${VRPlugins_install_dir}/${plugin_project_name})
	
	ExternalProject_add(BaseGLFW
			PREFIX ${MinVR_GLFW_prefix}
			GIT_REPOSITORY https://github.com/ivlab/BaseGLFW.git
			GIT_TAG master
			INSTALL_DIR ${VRPlugins_install_dir}/${plugin_project_name}
			CMAKE_ARGS ${project_CMAKE_ARGS}
			UPDATE_COMMAND ""
			SOURCE_DIR ${MinVR_GLFW_prefix}
			DOWNLOAD_DIR ${MinVR_GLFW_prefix}
			TMP_DIR ${VRPlugins_tmp_dir}/${plugin_project_name}
			STAMP_DIR ${VRPlugins_stamp_dir}/${plugin_project_name}
			BINARY_DIR ${VRPlugins_build_dir}/${plugin_project_name}
	)
	 
	set_property(TARGET BaseGLFW PROPERTY FOLDER "Plugins")
	
endif()

#-----------------Extensions--------------------------
if (false)
	set(MinVRExtensions_checkout_Dir ${PROJECT_SOURCE_DIR}/plugins/MinVRExtensions)
	make_directory(${MinVRExtensions_checkout_Dir})
	include(ExternalProject)
	
	set(MinVRExtensions_PREFIX "${MinVRExtensions_checkout_Dir}")
	set(MinVRExtensions_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/plugins")
	
	set(MinVRExtensions_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${MinVRExtensions_INSTALL_DIR} -DMinVR_INCLUDE_DIRS=${PROJECT_SOURCE_DIR}/.. -DMinVR_BINARY_DIRS=${CMAKE_BINARY_DIR}/lib -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
	set(MinVRExtensions_DIR "${MinVRExtensions_INSTALL_DIR}")
	
	if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		set(MinVRExtensions_CMAKE_ARGS ${MinVRExtensions_CMAKE_ARGS} -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD="c++11" -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY="libc++")
	endif()
	   
	ExternalProject_add(MinVRExtensions
			PREFIX ${MinVRExtensions_PREFIX}
			GIT_REPOSITORY https://github.com/ivlab/MinVRExtensions.git
			GIT_TAG minvr2
			INSTALL_DIR ${MinVRExtensions_INSTALL_DIR}
			CMAKE_ARGS ${MinVRExtensions_CMAKE_ARGS}
			UPDATE_COMMAND ""
	)
	 
	set_property(TARGET MinVRExtensions PROPERTY FOLDER "Plugins")
	
	add_dependencies(MinVRExtensions VREvent)
endif()

