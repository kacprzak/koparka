/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */

#include "TerrainGameState.h"

using namespace Ogre;

//------------------------------------------------------------------------------

TerrainGameState::TerrainGameState()
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

void TerrainGameState::enter()
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

bool TerrainGameState::pause()
{
    OgreFramework::getSingleton().log()->logMessage("Pausing TerrainGameState...");
    return true;
}

//------------------------------------------------------------------------------

void TerrainGameState::resume()
{
    OgreFramework::getSingleton().log()->logMessage("Resuming TerrainGameState...");

    buildGui();

    OgreFramework::getSingleton().viewport()->setCamera(m_camera);
    m_quit = false;
}

//------------------------------------------------------------------------------

void TerrainGameState::exit()
{
    OgreFramework::getSingleton().log()->logMessage("Leaving TerrainGameState...");

    m_sceneManager->destroyCamera(m_camera);
    m_sceneManager->destroyQuery(m_rsq);

    // Dziwne
    if (m_sceneManager)
        OgreFramework::getSingleton().root()->destroySceneManager(m_sceneManager);
}

//------------------------------------------------------------------------------

void TerrainGameState::createScene()
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

bool TerrainGameState::keyPressed(const OIS::KeyEvent& event)
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
        pushAppState(findByName("PauseState"));
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

bool TerrainGameState::keyReleased(const OIS::KeyEvent& event)
{
    // Just pass to OgreFramework
    OgreFramework::getSingleton().keyPressed(event);
    return true;
}

//------------------------------------------------------------------------------

bool TerrainGameState::mouseMoved(const OIS::MouseEvent& event)
{
    if (OgreFramework::getSingleton().trayManager()->injectMouseMove(event))
        return true;

    if (m_rightMouseDown) {
        m_camera->yaw(Degree(event.state.X.rel * -0.1f));
        m_camera->pitch(Degree(event.state.Y.rel * -0.1f));
    }

    return true;
}

//------------------------------------------------------------------------------

bool TerrainGameState::mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID id)
{
    if (OgreFramework::getSingleton().trayManager()->injectMouseDown(event, id))
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

bool TerrainGameState::mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID id)
{
    if (OgreFramework::getSingleton().trayManager()->injectMouseUp(event, id))
        return true;

    if (id == OIS::MB_Left) {
        m_leftMouseDown = false;
    } else if (id == OIS::MB_Right) {
        m_rightMouseDown = false;
    }

    return true;
}

//------------------------------------------------------------------------------

void TerrainGameState::onLeftPressed(const OIS::MouseEvent& event)
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

void TerrainGameState::moveCamera()
{
    if (OgreFramework::getSingleton().keyboard()->isKeyDown(OIS::KC_LSHIFT))
        m_camera->moveRelative(m_translateVector);

    m_camera->moveRelative(m_translateVector / 10);
}

//------------------------------------------------------------------------------

void TerrainGameState::getInput()
{
    if (m_settingsMode == false) {
        OgreFramework& of = OgreFramework::getSingleton();

        if (of.keyboard()->isKeyDown(OIS::KC_A))
            m_translateVector.x = -m_moveScale;

        if (of.keyboard()->isKeyDown(OIS::KC_D))
            m_translateVector.x = m_moveScale;

        if (of.keyboard()->isKeyDown(OIS::KC_W))
            m_translateVector.z = -m_moveScale;

        if (of.keyboard()->isKeyDown(OIS::KC_S))
            m_translateVector.z = m_moveScale;

        if (of.keyboard()->isKeyDown(OIS::KC_Q))
            m_translateVector.y = -m_moveScale;

        if (of.keyboard()->isKeyDown(OIS::KC_E))
            m_translateVector.y = m_moveScale;

        // Camera roll
        if (of.keyboard()->isKeyDown(OIS::KC_Z))
            m_camera->roll(Angle(-m_moveScale));

        if (of.keyboard()->isKeyDown(OIS::KC_X))
            m_camera->roll(Angle(m_moveScale));

        // Reset roll
        if (of.keyboard()->isKeyDown(OIS::KC_C))
            m_camera->roll(-(m_camera->getRealOrientation().getRoll()));
    }
}

//------------------------------------------------------------------------------

void TerrainGameState::buildGui()
{
    OgreFramework& of = OgreFramework::getSingleton();

    of.trayManager()->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    of.trayManager()->showLogo(OgreBites::TL_BOTTOMRIGHT);
    of.trayManager()->createLabel(OgreBites::TL_TOP, "GameLbl", "Game mode", 250);
    of.trayManager()->showCursor();

    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("Mode");

    m_detailsPanel = of.trayManager()->createParamsPanel(OgreBites::TL_TOPLEFT, "DetailsPanel",
                                                         200, items);
    m_detailsPanel->show();

    Ogre::String infoText = "[TAB] - Switch input mode\n\n"
        "[W] - Forward / Mode up\n"
        "[S] - Backwards / Mode down\n"
        "[A] - Left\n";
    infoText.append("[D] - Right\n\nPress [SHIFT] to move faster\n\n[O] - Toggle FPS / logo\n");
    infoText.append("[Print] - Take screenshot\n\n[ESC] - Exit");
    of.trayManager()->createTextBox(OgreBites::TL_RIGHT, "InfoPanel", infoText, 300, 200);

    Ogre::StringVector chatModes;
    chatModes.push_back("Solid mode");
    chatModes.push_back("Wireframe mode");
    chatModes.push_back("Point mode");
    of.trayManager()->createLongSelectMenu(OgreBites::TL_TOPRIGHT, "ChatModeSelMenu",
                                           "ChatMode", 200, 3, chatModes);
}

//------------------------------------------------------------------------------

void TerrainGameState::itemSelected(OgreBites::SelectMenu *menu)
{
    switch (menu->getSelectionIndex()) {
    case 0:
        m_camera->setPolygonMode(Ogre::PM_SOLID);
        break;
    case 1:
        m_camera->setPolygonMode(Ogre::PM_WIREFRAME);
        break;
    case 2:
        m_camera->setPolygonMode(Ogre::PM_POINTS);
        break;
    }
}

//------------------------------------------------------------------------------

void TerrainGameState::update(double timeSinceLastFrame)
{
#define TO_S StringConverter::toString

    m_frameEvent.timeSinceLastFrame = timeSinceLastFrame;

    OgreFramework::getSingleton().trayManager()->frameRenderingQueued(m_frameEvent);

    if (m_quit == true) {
        popAppState();
        return;
    }

    if (!OgreFramework::getSingleton().trayManager()->isDialogVisible()) {
        if (m_detailsPanel->isVisible()) {
            m_detailsPanel->setParamValue(0, TO_S(m_camera->getDerivedPosition().x));
            m_detailsPanel->setParamValue(1, TO_S(m_camera->getDerivedPosition().y));
            m_detailsPanel->setParamValue(2, TO_S(m_camera->getDerivedPosition().z));
            m_detailsPanel->setParamValue(3, TO_S(m_camera->getDerivedOrientation().w));
            m_detailsPanel->setParamValue(4, TO_S(m_camera->getDerivedOrientation().x));
            m_detailsPanel->setParamValue(5, TO_S(m_camera->getDerivedOrientation().y));
            m_detailsPanel->setParamValue(6, TO_S(m_camera->getDerivedOrientation().z));

            if (m_settingsMode) {
                m_detailsPanel->setParamValue(7, "Buffered Input");
            } else {
                m_detailsPanel->setParamValue(7, "Un-Buffered Input");                
            }
        }
    }

    m_moveScale = m_moveSpeed * timeSinceLastFrame;
    m_rotScale = m_rotateSpeed * timeSinceLastFrame;

    m_translateVector = Vector3::ZERO;

    getInput();
    moveCamera();

#undef TO_S
}

//------------------------------------------------------------------------------

void TerrainGameState::defineTerrain()
{}

//------------------------------------------------------------------------------

void TerrainGameState::initBlendMaps(Ogre::Terrain *terrain)
{}

//------------------------------------------------------------------------------

void TerrainGameState::configureTerrainDafaults(Ogre::Light *light)
{}
