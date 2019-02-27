cmake_minimum_required(VERSION 3.1)
project(RSA)

set (GCC_COVERAGE_COMPILE_FLAGS "-DTFM_DESC -DTFM_DESC -DLTM_DESC -DMP_32BIT -USE_TFM")

set(RSA_MAJOR_VERSION 0)
set(RSA_MINOR_VERSION 1)
set(RSA_PATCH_VERSION 0)
set(RSA_VERSION
	${RSA_MAJOR_VERSION}.${RSA_MINOR_VERSION}.${RSA_PATCH_VERSION}
	)
set (SRC
	src/rsaKey.cpp
	)

set (HEADER
	include/rsaKey.hpp
	)

add_library(RSA STATIC ${SRC} ${HEADER})
add_subdirectory(libtommath/)
add_subdirectory(libtomcrypt/)

set_property(TARGET RSA PROPERTY CXX_STANDARD 17)

target_link_libraries(RSA PRIVATE LTM)
target_link_libraries(RSA PRIVATE LTC)

target_include_directories(RSA PRIVATE ./include)
target_include_directories(RSA PRIVATE ./libtomcrypt/src/headers/.)
target_include_directories(RSA PRIVATE ./libtommath)
