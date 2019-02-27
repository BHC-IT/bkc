cmake_minimum_required(VERSION 3.1)
project(bfc)

set(bfc_MAJOR_VERSION 0)
set(bfc_MINOR_VERSION 1)
set(bfc_PATCH_VERSION 0)
set(bfc_VERSION
	${bfc_MAJOR_VERSION}.${bfc_MINOR_VERSION}.${bfc_PATCH_VERSION}
	)


set (SRC
	src/actorRep.cpp
	src/bfcmain.cpp
	src/exit.cpp
	src/flag.cpp
	src/masterThread.cpp
	src/usage.cpp
	)

set (HEADER
	bfc/actor.hpp
	bfc/actorRep.hpp
	bfc/bfc.hpp
	bfc/bfcmain.hpp
	bfc/exit.hpp
	bfc/factory.hpp
	bfc/flag.hpp
	bfc/masterThread.hpp
	bfc/outColor.hpp
	bfc/usage.hpp
	)

set (GCC_COVERAGE_COMPILE_FLAGS "-g3 -std=c++17 -O2 -fno-stack-protector -DNDEBUG -Wall -fno-rtti -DUSE_LTM -DLTM_DESC -DMP_32BIT")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(blc REQUIRED)

# add_library(frame ${SRC} ${HEADER})
add_library(frame STATIC ${SRC} ${HEADER})
target_include_directories(frame PRIVATE .)
target_link_libraries(frame PRIVATE Threads::Threads)
target_link_libraries(frame PRIVATE blc)

add_subdirectory(dev/)
add_subdirectory(dep/)

add_executable(bkcnode src/__dummy.cpp)

target_link_libraries(bkcnode PRIVATE Threads::Threads)
target_link_libraries(bkcnode PRIVATE blc)
target_link_libraries(bkcnode PRIVATE frame)
target_link_libraries(bkcnode PRIVATE devobj)
target_link_libraries(bkcnode PRIVATE RSA)
target_link_libraries(bkcnode PRIVATE LTC)
target_link_libraries(bkcnode PRIVATE LTM)
target_include_directories(bkcnode PRIVATE .)


add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
