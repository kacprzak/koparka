/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "DemoApp.h"

#include <iostream>
#include <exception>
 
//#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
//  #define WIN32_LEAN_AND_MEAN
//  #include "windows.h"
//  INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
//#else
  int main()
  //#endif
{
    DemoApp demo;
    try	{
        demo.startDemo();
    } catch(std::exception& e) {
        //#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        //MessageBoxA(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
        //#else
        std::cerr << "An exception has occurred: " << e.what() << std::endl;
        //#endif
    }
 
    return 0;
}
