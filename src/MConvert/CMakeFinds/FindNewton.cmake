# FIND Newton
# SET 
# Newton_INCLUDE_DIRS '-I'
# Newton_LIBRARY_DIRS '-L'
# Newton_LIBRARIES the '-l'


find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
	message("-- looking for Newton using PKG")
	pkg_check_modules(Newton Newton)
endif()


if (NOT Newton_FOUND)
	message("-- Manually looking for Newton")


	# Set Search Paths
	if (WIN32)
		set(Newton_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/sdk/include ${CMAKE_SOURCE_DIR}/sdk/include/Newton)
		set(Newton_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/sdk/lib)
	else (WIN32)
		set(Newton_INCLUDE_DIRS /usr/include/newtonSDK /usr/local/include/newtonSDK ${CMAKE_SOURCE_DIR}/libs/libNewton/linux64)
		set(Newton_LIBRARY_DIRS /usr/lib /usr/local/lib ${CMAKE_SOURCE_DIR}/libs/libNewton/linux64 )
	endif()
	
	find_path(Newton_INCLUDE_DIR Newton.h ${Newton_INCLUDE_DIRS})
	if (Newton_INCLUDE_DIR)
		message("--  Newton includes found")
	endif()


	find_library(Newton_LIBRARIES NAMES Newton PATHS ${Newton_LIBRARY_DIRS})
	if (Newton_LIBRARIES)
		message("--  Newton lib found")
	endif()


	if (Newton_INCLUDE_DIR AND Newton_LIBRARIES)
		message("--  Newton resolved!")
		set(Newton_FOUND TRUE)
	endif()	
endif()


if (NOT Newton_FOUND)
	message(FATAL_ERROR "Newton not found!")
endif()
