/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "OgreFramework.h"

//using namespace Ogre;

// Singleton initializaition
template<> OgreFramework* Ogre::Singleton<OgreFramework>::ms_Singleton = 0;

//------------------------------------------------------------------------------

OgreFramework::OgreFramework()
    : m_root(0)
    , m_window(0)
    , m_viewport(0)
    , m_log(0)
    , m_timer(0)
    , m_inputManager(0)
    , m_mouse(0)
    , m_keyboard(0)
    , m_trayManager(0)
{
}

//------------------------------------------------------------------------------

OgreFramework::~OgreFramework()
{
    OgreFramework::getSingletonPtr()->m_log->logMessage("Shutdown OGRE...");
    if (m_trayManager) delete m_trayManager;
    if (m_inputManager) OIS::InputManager::destroyInputSystem(m_inputManager);
    if (m_root) delete m_root;
}

//------------------------------------------------------------------------------

bool OgreFramework::initOgre(Ogre::String windowTitle, OIS::KeyListener *keyListener,
                             OIS::MouseListener *mouseListener)
{
    Ogre::LogManager *logMgr = new Ogre::LogManager();

    m_log = logMgr->createLog("OgreLogfile.log", true, true, false); 
    m_log->setDebugOutputEnabled(true);

    m_root = new Ogre::Root();

    // Okno konfiguracji jeśli nie ma zapisanych ustawień
    if (!m_root->restoreConfig() && !m_root->showConfigDialog())
        return false;

    // Auto create window
    m_window = m_root->initialise(true, windowTitle);

    // Creating viewport without a camera
    m_viewport = m_window->addViewport(0);
    m_viewport->setBackgroundColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f, 1.0f)); // Grey

    m_viewport->setCamera(0); // Again!?

    // Uchwyt do okna
    size_t hWnd = 0;
    m_window->getCustomAttribute("WINDOW", &hWnd);

    OIS::ParamList paramList;
    paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

    // Menadżer wejścia dla okna aplikacji
    m_inputManager = OIS::InputManager::createInputSystem(paramList);

    m_keyboard = static_cast<OIS::Keyboard*>(m_inputManager->createInputObject(OIS::OISKeyboard, true));
    m_mouse = static_cast<OIS::Mouse*>(m_inputManager->createInputObject(OIS::OISMouse, true));

    // Po co to?
    m_mouse->getMouseState().height = m_window->getHeight();
    m_mouse->getMouseState().width = m_window->getWidth();

    if (keyListener == 0)
        m_keyboard->setEventCallback(this);
    else
        m_keyboard->setEventCallback(keyListener);

    if (mouseListener == 0)
        m_mouse->setEventCallback(this);
    else
        m_mouse->setEventCallback(mouseListener);

    // Analiza pliku resources.cfg
    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements()) {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i) {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }

    // Ustawienie mipmap
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    // Incjalizacja grup zasobów (nie ładuje)
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    m_trayManager = new OgreBites::SdkTrayManager("AOFTrayMgr", m_window, m_mouse, 0);

    m_timer = new Ogre::Timer();
    m_timer->reset();

    m_window->setActive(true);

    return true;
}

//------------------------------------------------------------------------------

bool OgreFramework::keyPressed(const OIS::KeyEvent& /*event*/)
{
    // Screenshot
    if (m_keyboard->isKeyDown(OIS::KC_SYSRQ)) {
        m_window->writeContentsToTimestampedFile("AOF_Screenshot_", ".jpg");
        return true;
    }

    // Tray
    if (m_keyboard->isKeyDown(OIS::KC_O)) {
        if (m_trayManager->isLogoVisible()) {
            m_trayManager->hideFrameStats();
            m_trayManager->hideLogo();
        } else {
            m_trayManager->showFrameStats(OgreBites::TL_BOTTOMLEFT);
            m_trayManager->showLogo(OgreBites::TL_BOTTOMRIGHT);
        }
    }
    return true;
}

//------------------------------------------------------------------------------

bool OgreFramework::keyReleased(const OIS::KeyEvent& /*event*/)
{
    return true;
}

//------------------------------------------------------------------------------

bool OgreFramework::mouseMoved(const OIS::MouseEvent& /*event*/)
{
    return true;
}

//------------------------------------------------------------------------------

bool OgreFramework::mousePressed(const OIS::MouseEvent& /*event*/, OIS::MouseButtonID /*id*/)
{
    return true;
}

//------------------------------------------------------------------------------

bool OgreFramework::mouseReleased(const OIS::MouseEvent& /*event*/, OIS::MouseButtonID /*id*/)
{
    return true;
}

//------------------------------------------------------------------------------
/* Called once per frame by the AppStateManager to update everything directly
 * related to Ogre.
 */
void OgreFramework::updateOgre(double /*timeSinceLastFrame*/)
{
}

