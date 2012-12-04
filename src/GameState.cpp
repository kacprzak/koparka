/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */

#include "GameState.h"

using namespace Ogre;

//------------------------------------------------------------------------------

GameState::GameState()
    : m_quit(false)
    , m_detailsPanel(0)
    , m_moveSpeed(0.1f)
    , m_rotateSpeed(0.3f)
    , m_leftMouseDown(false)
    , m_rightMouseDown(false)
    , m_settingsMode(false)
{
}

//------------------------------------------------------------------------------

void GameState::enter()
{
    OgreFramework *of = OgreFramework::getSingletonPtr();

    m_sceneManager = of->root()->createSceneManager(ST_GENERIC, "GameSceneMgr");
    m_sceneManager->setAmbientLight(ColourValue(0.7f, 0.7f, 0.7f));

    m_rsq = m_sceneManager->createRayQuery(Ray());
    m_rsq->setQueryMask(OGRE_HEAD_MASK);

    m_camera = m_sceneManager->createCamera("GameCamera");
    m_camera->setPosition(Vector3(5, 60, 60));
    m_camera->lookAt(Vector3(5, 20, 0));
    m_camera->setNearClipDistance(5);

    m_camera->setAspectRatio(of->viewport()->getActualWidth()
                             / Real(of->viewport()->getActualHeight()));

    of->viewport()->setCamera(m_camera);
    m_currentNode = 0;

    buildGui();

    createScene();
}

//------------------------------------------------------------------------------

bool GameState::pause()
{
    OgreFramework::getSingletonPtr()->log()->logMessage("Pausing GameState...");
    return true;
}

//------------------------------------------------------------------------------

void GameState::resume()
{
    OgreFramework::getSingletonPtr()->log()->logMessage("Resuming GameState...");

    buildGui();

    OgreFramework::getSingletonPtr()->viewport()->setCamera(m_camera);
    m_quit = false;
}

//------------------------------------------------------------------------------

void GameState::exit()
{
    OgreFramework::getSingletonPtr()->log()->logMessage("Leaving GameState...");

    m_sceneManager->destroyCamera(m_camera);
    m_sceneManager->destroyQuery(m_rsq);

    // Dziwne
    if (m_sceneManager)
        OgreFramework::getSingletonPtr()->root()->destroySceneManager(m_sceneManager);
}

//------------------------------------------------------------------------------

void GameState::createScene()
{
    m_sceneManager->createLight("Light")->setPosition(75, 75, 75);

    DotSceneLoader *dotSceneLoader = new DotSceneLoader;
    dotSceneLoader->parseDotScene("CubeScene.xml", "General",
                                  m_sceneManager,
                                  m_sceneManager->getRootSceneNode());
    delete dotSceneLoader;

    m_sceneManager->getEntity("Cube01")->setQueryFlags(CUBE_MASK);
    m_sceneManager->getEntity("Cube02")->setQueryFlags(CUBE_MASK);
    m_sceneManager->getEntity("Cube03")->setQueryFlags(CUBE_MASK);

    m_ogreHeadEntity = m_sceneManager->createEntity("Cube", "ogrehead.mesh");
    m_ogreHeadEntity->setQueryFlags(OGRE_HEAD_MASK);

    m_ogreHeadNode = m_sceneManager->getRootSceneNode()->createChildSceneNode("CubeNode");
    m_ogreHeadNode->attachObject(m_ogreHeadEntity);
    m_ogreHeadNode->setPosition(Vector3(0, 0, -25));

    m_ogreHeadMat = m_ogreHeadEntity->getSubEntity(1)->getMaterial();
    m_ogreHeadMatHigh = m_ogreHeadMat->clone("OgreHeadMatHigh");
    m_ogreHeadMatHigh->getTechnique(0)->getPass(0)->setAmbient(1, 0, 0);
    m_ogreHeadMatHigh->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 0, 0);
}

//------------------------------------------------------------------------------

bool GameState::keyPressed(const OIS::KeyEvent& event)
{
    OgreFramework *of = OgreFramework::getSingletonPtr();

    if (m_settingsMode == true) {
        if (of->keyboard()->isKeyDown(OIS::KC_S)) {
            OgreBites::SelectMenu *menu =
                static_cast<OgreBites::SelectMenu*>(of->trayManager()->getWidget("ChatModeSelMenu"));
            if (menu->getSelectionIndex() + 1 < int(menu->getNumItems())) {
                menu->selectItem(menu->getSelectionIndex() + 1);
            }    
        }

        if (of->keyboard()->isKeyDown(OIS::KC_W)) {
            OgreBites::SelectMenu *menu =
                static_cast<OgreBites::SelectMenu*>(of->trayManager()->getWidget("ChatModeSelMenu"));
            if (menu->getSelectionIndex() - 1 >= 0) {
                menu->selectItem(menu->getSelectionIndex() - 1);
            }    
        }
    }

    if (of->keyboard()->isKeyDown(OIS::KC_ESCAPE)) {
        // pushAppState(findByName("PauseState"));
        return true;
    }

    if (of->keyboard()->isKeyDown(OIS::KC_I)) {
        if (m_detailsPanel->getTrayLocation() == OgreBites::TL_NONE) {
            of->trayManager()->moveWidgetToTray(m_detailsPanel, OgreBites::TL_TOPLEFT, 0);
            m_detailsPanel->show();
        } else {
            of->trayManager()->removeWidgetFromTray(m_detailsPanel);
            m_detailsPanel->hide();
        }
    }

    if (of->keyboard()->isKeyDown(OIS::KC_TAB)) {
        m_settingsMode = !m_settingsMode;
        return true;
    }

    if (m_settingsMode && of->keyboard()->isKeyDown(OIS::KC_RETURN)
        || of->keyboard()->isKeyDown(OIS::KC_NUMPADENTER)) {
        // Do nothing
    }

    if (!m_settingsMode || (m_settingsMode && !of->keyboard()->isKeyDown(OIS::KC_O))) {
        // Pass to OgreFramework
        of->keyPressed(event);
    }

    return true;
}

//------------------------------------------------------------------------------

bool GameState::keyReleased(const OIS::KeyEvent& event)
{
    // Just pass to OgreFramework
    OgreFramework::getSingletonPtr()->keyPressed(event);
    return true;
}

//------------------------------------------------------------------------------

bool GameState::mouseMoved(const OIS::MouseEvent& event)
{
    if (OgreFramework::getSingletonPtr()->trayManager()->injectMouseMove(event))
        return true;

    if (m_rightMouseDown) {
        m_camera->yaw(Degree(event.state.X.rel * -0.1f));
        m_camera->pitch(Degree(event.state.Y.rel * -0.1f));
    }

    return true;
}

//------------------------------------------------------------------------------

bool GameState::mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID id)
{
    if (OgreFramework::getSingletonPtr()->trayManager()->injectMouseDown(event, id))
        return true;

    if (id == OIS::MB_Left) {
        onLeftPressed(event);
        m_leftMouseDown = true;
    } else if (id == OIS::MB_Right) {
        m_rightMouseDown = true;
    }

    return true;
}

//------------------------------------------------------------------------------

bool GameState::mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID id)
{
    if (OgreFramework::getSingletonPtr()->trayManager()->injectMouseUp(event, id))
        return true;

    if (id == OIS::MB_Left) {
        m_leftMouseDown = false;
    } else if (id == OIS::MB_Right) {
        m_rightMouseDown = false;
    }

    return true;
}

//------------------------------------------------------------------------------

void GameState::onLeftPressed(const OIS::MouseEvent& event)
{
    OgreFramework *of = OgreFramework::getSingletonPtr();

    if (m_currentNode) {
        m_currentNode->showBoundingBox(false);
        m_currentEntity->getSubEntity(1)->setMaterial(m_ogreHeadMat);
    }

    Ray mouseRay = m_camera->getCameraToViewportRay(of->mouse()->getMouseState().X.abs / float(event.state.width),
                                                    of->mouse()->getMouseState().Y.abs / float(event.state.height));
    m_rsq->setRay(mouseRay);
    m_rsq->setSortByDistance(true);

    RaySceneQueryResult& result = m_rsq->execute();
    RaySceneQueryResult::iterator it;

    for (it = result.begin(); it != result.end(); ++it) {
        if (it->movable) {
            of->log()->logMessage("MovableName: " + it->movable->getName());
            m_currentNode = m_sceneManager->getEntity(it->movable->getName())->getParentSceneNode();
            of->log()->logMessage("ObjName " + m_currentNode->getName());
            m_currentNode->showBoundingBox(true);
            m_currentEntity = m_sceneManager->getEntity(it->movable->getName());
            m_currentEntity->getSubEntity(1)->setMaterial(m_ogreHeadMatHigh);
            break;
        }
    }
}

//------------------------------------------------------------------------------

void GameState::moveCamera()
{}

//------------------------------------------------------------------------------

void GameState::getInput()
{}

//------------------------------------------------------------------------------

void GameState::buildGui()
{}

//------------------------------------------------------------------------------

void GameState::itemSelected(OgreBites::SelectMenu *menu)
{}

//------------------------------------------------------------------------------

void GameState::update(double timeSinceLastFrame)
{}

