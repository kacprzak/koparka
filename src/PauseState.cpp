/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "PauseState.h"

using namespace Ogre;

//------------------------------------------------------------------------------

PauseState::PauseState()
    : m_quit(false)
    , m_questionActive(false)
{
    m_frameEvent = Ogre::FrameEvent();
}

//------------------------------------------------------------------------------

void PauseState::enter()
{
    OgreFramework& of = OgreFramework::getSingleton();

    of.log()->logMessage("Entering PauseState...");

    m_sceneManager = of.root()->createSceneManager(ST_GENERIC, "PauseSceneMgr");
    m_sceneManager->setAmbientLight(ColourValue(0.7f, 0.7f, 0.7f));

    m_camera = m_sceneManager->createCamera("PauseCam");
    m_camera->setPosition(Vector3(0, 25, -50));
    m_camera->lookAt(Vector3(0, 0, 0));
    m_camera->setNearClipDistance(1);

    m_camera->setAspectRatio(of.viewport()->getActualWidth() /
                             Real(of.viewport()->getActualHeight()));

    of.viewport()->setCamera(m_camera);

    of.trayManager()->destroyAllWidgets();
    of.trayManager()->showCursor();
    of.trayManager()->createButton(OgreBites::TL_CENTER, "BackToGameBtn", "Return To Game State", 250);
    of.trayManager()->createButton(OgreBites::TL_CENTER, "BackToMenuBtn", "Return To Menu", 250);
    of.trayManager()->createButton(OgreBites::TL_CENTER, "ExitBtn", "Exit OgreFramework", 250);
    of.trayManager()->createLabel(OgreBites::TL_TOP, "PauseLbl", "PauseMode", 250);

    m_quit = false;

    createScene();
}

//------------------------------------------------------------------------------

void PauseState::createScene()
{
    // Nothing to create
}

//------------------------------------------------------------------------------

void PauseState::exit()
{
    OgreFramework& of = OgreFramework::getSingleton();

    of.log()->logMessage("Leaving PauseState...");

    m_sceneManager->destroyCamera(m_camera);
    if (m_sceneManager)
        of.root()->destroySceneManager(m_sceneManager);

    of.trayManager()->clearAllTrays();
    of.trayManager()->destroyAllWidgets();
    of.trayManager()->setListener(0);
}

//------------------------------------------------------------------------------

bool PauseState::keyPressed(const OIS::KeyEvent& event)
{
    if (OgreFramework::getSingleton().keyboard()->isKeyDown(OIS::KC_ESCAPE) && !m_questionActive) {
        m_quit = true;
        return true;
    }

    OgreFramework::getSingleton().keyPressed(event);

    return true;
}

//------------------------------------------------------------------------------

bool PauseState::keyReleased(const OIS::KeyEvent& event)
{
    OgreFramework::getSingleton().keyReleased(event);

    return true;
}

//------------------------------------------------------------------------------

bool PauseState::mouseMoved(const OIS::MouseEvent& event)
{
    if (OgreFramework::getSingleton().trayManager()->injectMouseMove(event))
        return true;

    return true;
}

//------------------------------------------------------------------------------

bool PauseState::mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID id)
{
    if (OgreFramework::getSingleton().trayManager()->injectMouseDown(event, id))
        return true;

    return true;
}

//------------------------------------------------------------------------------

bool PauseState::mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID id)
{
    if (OgreFramework::getSingleton().trayManager()->injectMouseUp(event, id))
        return true;

    return true;
}

//------------------------------------------------------------------------------

void PauseState::update(double timeSinceLastFrame)
{
    m_frameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingleton().trayManager()->frameRenderingQueued(m_frameEvent);

    if (m_quit == true) {
        popAppState();
        return;
    }
}

//------------------------------------------------------------------------------

void PauseState::buttonHit(OgreBites::Button *button)
{
    if (button->getName() == "ExitBtn") {
        OgreFramework::getSingleton().trayManager()->showYesNoDialog("Sure?", "Really leave?");
        m_questionActive = true;
    } else if (button->getName() == "BackToGameBtn") {
        popAllAndPushAppState(findByName("GameState"));
        m_quit = true;
    } else if (button->getName() == "BackToMenuBtn") {
        popAllAndPushAppState(findByName("MenuState"));
    }
}

//------------------------------------------------------------------------------

void PauseState::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit)
{
    if (yesHit == true)
        shutdown();
    else
        OgreFramework::getSingleton().trayManager()->closeDialog();

    m_questionActive = false;
}
