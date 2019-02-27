cmake_minimum_required(VERSION 3.1)
project(DEP)


file(GLOB files "*")
foreach(file ${files})
	if (IS_DIRECTORY ${file})
		if (NOT ${file} MATCHES ${CMAKE_BINARY_DIR})
			add_subdirectory(${file})
		endif()
	endif()
endforeach()