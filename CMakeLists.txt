cmake_minimum_required( VERSION 2.8 )
 
project( koparka )

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

if( MSVC )
  #add_definitions( "" )
else()
  add_definitions( "-Wall -Wextra -g -std=c++0x" )
endif()


find_package( OGRE REQUIRED )
find_package( OIS REQUIRED )

set( CMAKE_DEBUG_POSTFIX "_d" )

set( HDRS

)
 
set( SRCS
        src/main.cpp
        src/OgreFramework.cpp
        src/AppStateManager.cpp
        src/DemoApp.cpp
        src/MenuState.cpp
        src/GameState.cpp
)

add_subdirectory( external )

include_directories(
        /usr/include/OGRE
        external
        src
)

set( OGRE_LIBRARIES
        OgreMain
        OgreTerrain
)
set( OIS_LIBRARIES
        OIS
)

add_executable( ${PROJECT_NAME} ${HDRS} ${SRCS} )

set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)
 
target_link_libraries( ${PROJECT_NAME} DotSceneLoader ${OGRE_LIBRARIES} ${OIS_LIBRARIES})

file( COPY ${CMAKE_SOURCE_DIR}/resources.cfg DESTINATION ${CMAKE_BINARY_DIR}/ )
file( COPY ${CMAKE_SOURCE_DIR}/plugins.cfg DESTINATION ${CMAKE_BINARY_DIR}/ )
file( COPY ${CMAKE_SOURCE_DIR}/media DESTINATION ${CMAKE_BINARY_DIR}/ )

