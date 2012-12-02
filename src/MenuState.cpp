/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "MenuState.h"

using namespace Ogre;

//------------------------------------------------------------------------------

MenuState::MenuState()
    : m_quit(false)
{
    m_frameEvent = Ogre::FrameEvent();
}

//------------------------------------------------------------------------------

void MenuState::enter()
{
    OgreFramework *of = OgreFramework::getSingletonPtr();

    of->m_log->logMessage("Entering MenuState...");

    m_sceneManager = of->m_root->createSceneManager(ST_GENERIC, "MenuSceneMgr");
    m_sceneManager->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

    m_camera = m_sceneManager->createCamera("MenuCam");
    m_camera->setPosition(Vector3(0, 25, -50));
    m_camera->lookAt(Vector3(0, 0, 0));
    m_camera->setNearClipDistance(1);

    m_camera->setAspectRatio(of->m_viewport->getActualWidth() /
                             Real(of->m_viewport->getActualHeight()));

    of->m_viewport->setCamera(m_camera);

    of->m_trayManager->destroyAllWidgets();
    of->m_trayManager->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    of->m_trayManager->showLogo(OgreBites::TL_BOTTOMRIGHT);
    of->m_trayManager->showCursor();
    of->m_trayManager->createButton(OgreBites::TL_CENTER, "EnterBtn",
                                    "Enter GameState", 250);
    of->m_trayManager->createButton(OgreBites::TL_CENTER, "ExitBtn",
                                    "Exit OgreFramework", 250);
    of->m_trayManager->createLabel(OgreBites::TL_TOP, "MenuLbl",
                                   "Menu mode", 250);

    createScene();
}

//------------------------------------------------------------------------------

void MenuState::createScene()
{
    // No content in MenuState
}

//------------------------------------------------------------------------------

void MenuState::exit()
{
    OgreFramework *of = OgreFramework::getSingletonPtr();

    of->m_log->logMessage("Leaving MenuState...");

    m_sceneManager->destroyCamera(m_camera);
    if(m_sceneManager)
        of->m_root->destroySceneManager(m_sceneManager);

    of->m_trayManager->clearAllTrays();
    of->m_trayManager->destroyAllWidgets();
    of->m_trayManager->setListener(0);
}

//------------------------------------------------------------------------------

void MenuState::update(double timeSinceLastFrame)
{
    m_frameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_trayManager->frameRenderingQueued(m_frameEvent);

    if (m_quit == true) {
        shutdown();
        return;
    }
}

//------------------------------------------------------------------------------

bool MenuState::keyPressed(const OIS::KeyEvent& event)
{
    if (OgreFramework::getSingletonPtr()->m_keyboard->isKeyDown(OIS::KC_ESCAPE)) {
        m_quit = true;
        return true;
    }

    OgreFramework::getSingletonPtr()->keyPressed(event);
    return true;
}

//------------------------------------------------------------------------------

bool MenuState::keyReleased(const OIS::KeyEvent& event)
{
    OgreFramework::getSingletonPtr()->keyReleased(event);
    return true;
}

//------------------------------------------------------------------------------

bool MenuState::mouseMoved(const OIS::MouseEvent& event)
{
    if (OgreFramework::getSingletonPtr()->m_trayManager->injectMouseMove(event))
        return true;

    return true;
}

//------------------------------------------------------------------------------

bool MenuState::mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID id)
{
    if (OgreFramework::getSingletonPtr()->m_trayManager->injectMouseDown(event, id))
        return true;

    return true;
}

//------------------------------------------------------------------------------

bool MenuState::mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID id)
{
    if (OgreFramework::getSingletonPtr()->m_trayManager->injectMouseUp(event, id))
        return true;

    return true;
}

//------------------------------------------------------------------------------

/**
 * Callback function that gets triggered when a button is hit.
 */
void MenuState::buttonHit(OgreBites::Button *button)
{
    if (button->getName() == "ExitBtn")
        m_quit = true;
    else if (button->getName() == "EnterBtn")
        changeAppState(findByName("GameState"));
}
