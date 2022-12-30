#################################################################################################
## File created by QxEntityEditor 1.2.6 (2022/12/29 03:00) : please, do NOT modify this file ! ##
#################################################################################################

cmake_minimum_required(VERSION 3.1)

project(bridgeServer LANGUAGES CXX)

if(NOT TARGET QxOrm)
   add_subdirectory("../../../../Qt/libraries/QxOrm/" ./QxOrm)
endif() # NOT TARGET QxOrm

include("../../../../Qt/libraries/QxOrm/QxOrm.cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_DEBUG_POSTFIX "d")

set(BRIDGESERVER_HEADERS
    ./include/bridgeServer_precompiled_header.gen.h
    ./include/bridgeServer_export.gen.h
    ./include/Friends.gen.h
    ./include/History.gen.h
    ./include/Users.gen.h
    ./include/QuestionsType.gen.h
   )

if(_QX_UNITY_BUILD)

   set(BRIDGESERVER_SRCS ./src/bridgeServer_all.gen.cpp)

else() # _QX_UNITY_BUILD

   set(BRIDGESERVER_SRCS
       ./src/Friends.gen.cpp
       ./src/History.gen.cpp
       ./src/Users.gen.cpp
       ./src/QuestionsType.gen.cpp
       ./src/bridgeServer_main.gen.cpp
      )

endif() # _QX_UNITY_BUILD

add_library(bridgeServer SHARED ${BRIDGESERVER_SRCS} ${BRIDGESERVER_HEADERS})

target_compile_definitions(bridgeServer PRIVATE -D_BUILDING_BRIDGESERVER)

if(COMMAND target_precompile_headers)
   target_precompile_headers(bridgeServer PRIVATE ./include/bridgeServer_precompiled_header.gen.h)
endif() # (COMMAND target_precompile_headers)

target_link_libraries(bridgeServer ${QX_LIBRARIES} QxOrm)

set_target_properties(bridgeServer PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
                      ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin"
                      LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin"
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin"
                      ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin"
                      LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin"
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin"
                      ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_SOURCE_DIR}/bin"
                      LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_SOURCE_DIR}/bin"
                      RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_SOURCE_DIR}/bin"
                      ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_SOURCE_DIR}/bin"
                      LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_SOURCE_DIR}/bin"
                      RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_SOURCE_DIR}/bin"
                     )

qx_auto_source_group("${BRIDGESERVER_SRCS};${BRIDGESERVER_HEADERS}")
