cmake_minimum_required(VERSION 3.1)
project(bfc)

set(BFC_MAJOR_VERSION 0)
set(BFC_MINOR_VERSION 5)
set(BFC_PATCH_VERSION 0)
set(BFC_VERSION
	${BFC_MAJOR_VERSION}.${BFC_MINOR_VERSION}.${BFC_PATCH_VERSION}
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

set(GCC_COVERAGE_COMPILE_FLAGS "-O2 -fno-stack-protector -DNDEBUG -Wall -fno-rtti -DUSE_LTM -DLTM_DESC -DMP_32BIT")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(blc REQUIRED)

# add_library(frame ${SRC} ${HEADER})
add_library(frame STATIC ${SRC} ${HEADER})
target_include_directories(frame PRIVATE .)
target_link_libraries(frame PRIVATE Threads::Threads)
target_link_libraries(frame PRIVATE blc)
target_link_libraries(frame PRIVATE RSA)
target_link_libraries(frame PRIVATE LTC)
target_link_libraries(frame PRIVATE LTM)
target_include_directories(frame PRIVATE .)
target_include_directories(frame PRIVATE ./dep/rsa/libtomcrypt/src/headers/.)
target_include_directories(frame PRIVATE ./dep/rsa/libtommath)

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
target_include_directories(bkcnode PRIVATE ./dep/rsa/libtomcrypt/src/headers/.)
target_include_directories(bkcnode PRIVATE ./dep/rsa/libtommath)

target_compile_definitions(bkcnode PRIVATE BFC_MAJOR_VERSION=${BFC_MAJOR_VERSION})
target_compile_definitions(bkcnode PRIVATE BFC_MINOR_VERSION=${BFC_MINOR_VERSION})
target_compile_definitions(bkcnode PRIVATE BFC_PATCH_VERSION=${BFC_PATCH_VERSION})

target_compile_definitions(frame PRIVATE BFC_MAJOR_VERSION=${BFC_MAJOR_VERSION})
target_compile_definitions(frame PRIVATE BFC_MINOR_VERSION=${BFC_MINOR_VERSION})
target_compile_definitions(frame PRIVATE BFC_PATCH_VERSION=${BFC_PATCH_VERSION})

set_property(TARGET frame PROPERTY CXX_STANDARD 11)
set_property(TARGET bkcnode PROPERTY CXX_STANDARD 17)


add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
