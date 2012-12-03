cmake_minimum_required( VERSION 2.8 )
 
project( koparka )

if(WIN32)
	set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
	set(OGRE_SAMPLES_INCLUDEPATH
		$ENV{OGRE_HOME}/Samples/include
	)
endif(WIN32)
 
if(UNIX)
	if(EXISTS "/usr/lib64/OGRE/cmake")
                  set(CMAKE_MODULE_PATH "/usr/lib64/OGRE/cmake/;${CMAKE_MODULE_PATH}")
                  # We could just *assume* that developers uses this basepath : /usr/local
	          set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/")
	elseif(EXISTS "/usr/lib/OGRE/cmake")
	          set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
                  # Otherwise, this one
                  set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/")
	else ()
	          message(SEND_ERROR "Failed to find module path.")
	endif(EXISTS "/usr/lib64/OGRE/cmake")
endif(UNIX)

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

