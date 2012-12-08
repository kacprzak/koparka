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

if (OGRE_Terrain_FOUND)
  # pretty sure the following include is unneeded
  # include_directories(${OGRE_Terrain_INCLUDE_DIRS})
  # append OgreTerrain to the end of the OGRE_LIBRARIES variable
  set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${OGRE_Terrain_LIBRARIES})  
  message(STATUS "Found OGRE_Terrain: ${OGRE_Terrain_LIBRARIES}")
else (OGRE_Terrain_FOUND)
  message(SEND_ERROR "OgreTerrain Library not found.")
endif(OGRE_Terrain_FOUND)

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
        src/PauseState.cpp
        src/TerrainGameState.cpp
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

