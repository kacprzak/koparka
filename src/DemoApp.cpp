/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "DemoApp.h"

#include "AppStateManager.h"
#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"

//------------------------------------------------------------------------------

DemoApp::DemoApp()
    : m_appStateManager(0)
{
}

//------------------------------------------------------------------------------

DemoApp::~DemoApp()
{
    delete m_appStateManager;
    delete OgreFramework::getSingletonPtr();
}

//------------------------------------------------------------------------------

void DemoApp::startDemo()
{
    new OgreFramework();
    if (!OgreFramework::getSingletonPtr()->initOgre("OgreFramework", 0, 0))
        return;

    OgreFramework::getSingletonPtr()->log()->logMessage("Demo initialized");

    m_appStateManager = new AppStateManager();

    MenuState::create(m_appStateManager, "MenuState");
    GameState::create(m_appStateManager, "GameState");
    PauseState::create(m_appStateManager, "PauseState");

    m_appStateManager->start(m_appStateManager->findByName("MenuState"));
}
