/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef APP_STATE_H
#define APP_STATE_H

#include "OgreFramework.h"

class AppState;

//------------------------------------------------------------------------------

class AppStateListener
{
 public:
    AppStateListener() {}
    virtual ~AppStateListener() {}

    // Function to later add a new state to manager
    virtual void manageAppState(Ogre::String stateName, AppState *state) = 0;

    virtual AppState* findByName(Ogre::String stateName) = 0;

    virtual void changeAppState(AppState *state) = 0;
    virtual bool pushAppState(AppState *state) = 0;
    virtual void popAppState() = 0;
    virtual void pauseAppState() = 0;
    virtual void shutdown() = 0;
    virtual void popAllAndPushAppState(AppState *state) = 0;
};

//------------------------------------------------------------------------------

class AppState : public OIS::KeyListener, public OIS::MouseListener,
    public OgreBites::SdkTrayListener
{
 public:
    //static void create(AppStateListener *parent, const Ogre::String name) {}

    void destroy() { delete this; }

    // Callbacks used by AppStateManager
    /** Called when state is put on the active states stack */
    virtual void enter() = 0;
    /** Called when state is removed from active states stack */
    virtual void exit() = 0;
    /** Called when other is trying to pause state. Should return
        flase if state can't be paused. */
    virtual bool pause() { return true; }
    /** Reverse of pause */
    virtual void resume() {}
    /** Called once on every frame */
    virtual void update(double timeSinceLastFrame) = 0;

 protected:
    AppState() {}

    AppState* findByName(Ogre::String stateName)
    {
        return m_parent->findByName(stateName);
    }

    void changeAppState(AppState *state) { m_parent->changeAppState(state); }

    bool pushAppState(AppState *state)
    {
        return m_parent->pushAppState(state);
    }

    void popAppState() { m_parent->popAppState(); }
    void shutdown() { m_parent->shutdown(); }

    void popAllAndPushAppState(AppState *state)
    {
        m_parent->popAllAndPushAppState(state);
    }

    // Fields
    AppStateListener *m_parent;
    
    Ogre::Camera *m_camera;
    Ogre::SceneManager *m_sceneManager;

    Ogre::FrameEvent m_frameEvent;
};

//------------------------------------------------------------------------------

#define DECLARE_APPSTATE_CLASS(T)                                       \
    static void create(AppStateListener *parent, const Ogre::String name) \
    {                                                                   \
    T* myAppState = new T();                                            \
    myAppState->m_parent = parent;                                      \
    parent->manageAppState(name, myAppState);                           \
    }

//------------------------------------------------------------------------------

#endif
