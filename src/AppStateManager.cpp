/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "AppStateManager.h"

#include <OGRE/OgreWindowEventUtilities.h>

//------------------------------------------------------------------------------

AppStateManager::AppStateManager()
  : m_shutdown(false)
{}

//------------------------------------------------------------------------------

AppStateManager::~AppStateManager()
{
    while (!m_activeStateStack.empty()) {
        m_activeStateStack.back()->exit();
        m_activeStateStack.pop_back();
    }

    while (!m_states.empty()) {
        state_info si = m_states.back();
        si.state->destroy();
        m_states.pop_back();
    }
}

//------------------------------------------------------------------------------

/**
 * Called from within the state creation macro and sets some
 * information of the new state, as well as pushing it on the active
 * states stack.
 */
void AppStateManager::manageAppState(Ogre::String stateName, AppState *state)
{
    try {
        state_info si;
        si.name = stateName;
        si.state = state;
        m_states.push_back(si);
    } catch (std::exception& e) {
        delete state;
        throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,
                              "Error while tring to manage a new AppState\n"
                              + Ogre::String(e.what()),
                              __FILE__ ": __LINE__");
    }
}

//------------------------------------------------------------------------------

/**
 * Returns a pointer to the state with the respective name.
 */
AppState* AppStateManager::findByName(Ogre::String stateName)
{
    for (const state_info& si : m_states) {
        if (si.name == stateName) {
            return si.state;
        }
    }
    return 0;
}

//------------------------------------------------------------------------------

/**
 * Application main loop.
 */
void AppStateManager::start(AppState *state)
{
    OgreFramework *of = OgreFramework::getSingletonPtr();

    changeAppState(state);

    int timeSinceLastFrame = 1;
    int startTime = 0;

    while (!m_shutdown) {
        if (of->m_window->isClosed())
            m_shutdown = true;

        Ogre::WindowEventUtilities::messagePump();

        if (of->m_window->isActive()) {
            startTime = of->m_timer->getMillisecondsCPU();

            of->m_keyboard->capture();
            of->m_mouse->capture();

            m_activeStateStack.back()->update(timeSinceLastFrame);
            
            of->updateOgre(timeSinceLastFrame);
            of->m_root->renderOneFrame();

            timeSinceLastFrame = of->m_timer->getMillisecondsCPU() - startTime;
        } else {
            //#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            //Sleep(1000);
            //#else
            sleep(1);
            //#endif
        }
    }

    of->m_log->logMessage("Main loop quit");
}

//------------------------------------------------------------------------------

/**
 * Exits the current state (if there is any) and starts the new specified one.
 */
void AppStateManager::changeAppState(AppState *state)
{
    if (!m_activeStateStack.empty()) {
        m_activeStateStack.back()->exit();
        m_activeStateStack.pop_back();
    }

    m_activeStateStack.push_back(state);
    init(state);
    m_activeStateStack.back()->enter();
}

//------------------------------------------------------------------------------

/**
 * Puts a new state on the top of the stack and starts it. Returns false if
 * current state can not be paused.
 */
bool AppStateManager::pushAppState(AppState *state)
{
    if (!m_activeStateStack.empty()) {
        if (!m_activeStateStack.back()->pause())
            return false;
    }

    m_activeStateStack.push_back(state);
    init(state);
    m_activeStateStack.back()->enter();

    return true;
}

//------------------------------------------------------------------------------

/**
 * Removes the top most state and resumes the one belowe if there is one,
 * otherwise shutdown the application.
 */
void AppStateManager::popAppState()
{
    if (!m_activeStateStack.empty()) {
        m_activeStateStack.back()->exit();
        m_activeStateStack.pop_back();
    }

    if (!m_activeStateStack.empty()) {
        init(m_activeStateStack.back());
        m_activeStateStack.back()->resume();
    } else {
        shutdown();
    }
}

//------------------------------------------------------------------------------

/**
 * Exits all existing app states on the stack and enters the given new state.
 */
void AppStateManager::popAllAndPushAppState(AppState *state)
{
    while (!m_activeStateStack.empty()) {
        m_activeStateStack.back()->exit();
        m_activeStateStack.pop_back();
    }

    pushAppState(state);
}

//------------------------------------------------------------------------------

/**
 * Pauses the current app state and resumes the one in the stack below.
 */
void AppStateManager::pauseAppState()
{
    if (!m_activeStateStack.empty()) {
        m_activeStateStack.back()->pause();
    }

    if (m_activeStateStack.size() > 2) {
        init(m_activeStateStack.at(m_activeStateStack.size() - 2));
        m_activeStateStack.at(m_activeStateStack.size() - 2)->resume();
    }
}

//------------------------------------------------------------------------------

/**
 * Exits the application.
 */
void AppStateManager::shutdown()
{
    m_shutdown = true;
}

//------------------------------------------------------------------------------

void AppStateManager::init(AppState *state)
{
    OgreFramework *of = OgreFramework::getSingletonPtr();

    of->m_keyboard->setEventCallback(state);
    of->m_mouse->setEventCallback(state);
    of->m_trayManager->setListener(state);

    of->m_window->resetStatistics();
}
