/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef OGRE_FRAMEWORK_H
#define OGRE_FRAMEWORK_H

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreLogManager.h>
#include <OGRE/OgreOverlay.h>
#include <OGRE/OgreOverlayElement.h>
#include <OGRE/OgreOverlayManager.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreViewport.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreConfigFile.h>
 
#include <OIS/OISEvents.h>
#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>

#include <OGRE/SdkTrays.h>
 
class OgreFramework : public Ogre::Singleton<OgreFramework>,
    OIS::KeyListener, OIS::MouseListener
{
 public:
    OgreFramework();
    ~OgreFramework();

    bool initOgre(Ogre::String windowTitle, OIS::KeyListener *keyListener = 0,
                  OIS::MouseListener *mouseListener = 0);
    void updateOgre(double timeSinceLastFrame);

    // KeyListener interface
    bool keyPressed(const OIS::KeyEvent& keyEvent);
    bool keyReleased(const OIS::KeyEvent& keyEvent);

    // MouseListener interface
    bool mouseMoved(const OIS::MouseEvent& event);
    bool mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID id);

    // Getters
    Ogre::Root* root() { return m_root; }
    Ogre::RenderWindow* renderWindow() { return m_window; }
    Ogre::Viewport* viewport() { return m_viewport; }
    Ogre::Log* log() { return m_log; }
    Ogre::Timer* timer() { return m_timer; }

    OIS::InputManager* inuptManager() { return m_inputManager; }
    OIS::Mouse* mouse() { return m_mouse; }
    OIS::Keyboard* keyboard() { return m_keyboard; }

    OgreBites::SdkTrayManager* trayManager() { return m_trayManager; }

 private:
    OgreFramework(const OgreFramework& of); // Don't implement
    void operator=(const OgreFramework& of); // Don't implement
 
    // Główny obiekt silnika OGRE
    Ogre::Root *m_root;
    // Obiekt okna na którym rysowana jest scena
    Ogre::RenderWindow *m_window;
    // Obszar w którym wyświetlana będzie scena
    Ogre::Viewport *m_viewport;
    // Logger
    Ogre::Log *m_log;
    // Zegar
    Ogre::Timer *m_timer;

    // Wejście
    OIS::InputManager *m_inputManager;
    OIS::Mouse *m_mouse;
    OIS::Keyboard *m_keyboard;

    // Wyświetla dodatkowie informacje o silniku
    OgreBites::SdkTrayManager *m_trayManager;
};

#endif
