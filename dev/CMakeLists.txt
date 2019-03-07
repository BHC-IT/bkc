cmake_minimum_required(VERSION 3.1)
project(dev)

set(BKC_MAJOR_VERSION 1)
set(BKC_MINOR_VERSION 0)
set(BKC_PATCH_VERSION 0)
set(BKC_VERSION
	${BKC_MAJOR_VERSION}.${BKC_MINOR_VERSION}.${BKC_PATCH_VERSION}
	)

add_compile_definitions(BKC_MAJOR_VERSION=${BKC_MAJOR_VERSION})
add_compile_definitions(BKC_MINOR_VERSION=${BKC_MINOR_VERSION})
add_compile_definitions(BKC_PATCH_VERSION=${BKC_PATCH_VERSION})

set (GCC_COVERAGE_COMPILE_FLAGS "-DTFM_DESC -DTFM_DESC -DLTM_DESC -DMP_32BIT -USE_TFM")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(blc REQUIRED)

set (SRCS
	src/adminConnector.cpp
	src/book.cpp
	src/chain.cpp
	src/cinConnector.cpp
	src/confFile.cpp
	src/connectedPeer.cpp
	src/getMyIp.cpp
	src/handleKeyParam.cpp
	src/httpPost.cpp
	src/initChain.cpp
	src/lists.cpp
	src/main.cpp
	src/peerConnector.cpp
	src/peerServer.cpp
	src/servConnector.cpp
	src/trans.cpp
	)

set (PROTOS
	protocols/protocolAdminFromAdmin.cpp
	protocols/protocolAdminFromMaster.cpp
	protocols/protocolChainFromMaster.cpp
	protocols/protocolCinFromCin.cpp
	protocols/protocolCinFromMaster.cpp
	protocols/protocolMasterFromAdmin.cpp
	protocols/protocolMasterFromChain.cpp
	protocols/protocolMasterFromCin.cpp
	protocols/protocolMasterFromPeer.cpp
	protocols/protocolMasterFromServer.cpp
	protocols/protocolPeerFromMaster.cpp
	protocols/protocolPeerFromPeer.cpp
	protocols/protocolServerFromMaster.cpp
	)

set (HEADER
	include/adminConnector.hpp
	include/book.hpp
	include/chain.hpp
	include/chainInfo.hpp
	include/cinConnector.hpp
	include/coInfo.hpp
	include/confFile.hpp
	include/connectedPeer.hpp
	include/getMyIp.hpp
	include/handleKeyParam.hpp
	include/identity.hpp
	include/initChain.hpp
	include/lists.hpp
	include/httpPost.hpp
	include/meta.hpp
	include/peerConnector.hpp
	include/peerServer.hpp
	include/servConnector.hpp
	include/trans.hpp
	)

add_library(devobj STATIC ${SRCS} ${HEADER} ${PROTOS})

target_include_directories(devobj PRIVATE ./include)
target_include_directories(devobj PRIVATE ../)
target_include_directories(devobj PRIVATE ../include/.)
target_include_directories(devobj PRIVATE ../dep/rsa/libtomcrypt/src/headers/.)
target_include_directories(devobj PRIVATE ../dep/rsa/libtommath)
target_include_directories(devobj PRIVATE ../dep/rsa/include)

target_link_libraries(devobj PRIVATE Threads::Threads)
target_link_libraries(devobj PRIVATE blc)

set_property(TARGET devobj PROPERTY CXX_STANDARD 17)

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
