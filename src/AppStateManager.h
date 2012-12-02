/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef APP_STATE_MANAGER_H
#define APP_STATE_MANAGER_H

#include "AppState.h"

/**
 * The class AppStateManager inherits from the class AppStateListener
 * shown above and mainly implements its abstract methods. Furthermore
 * it contains:
 *
 *   std::vector for all existing states (m_states)
 *   std::vector for the active states, so the stack of those states
 *               currently is use (m_activeStateStack)
 */
class AppStateManager : public AppStateListener
{
 public:
    typedef struct {
        Ogre::String name;
        AppState *state;
    } state_info;

    AppStateManager();
    ~AppStateManager();

    void start(AppState *state);

    // AppStateListener interface
    void manageAppState(Ogre::String stateName, AppState *state);

    AppState* findByName(Ogre::String stateName);
    
    void changeAppState(AppState *state);
    bool pushAppState(AppState *state);
    void popAppState();
    void pauseAppState();
    void shutdown();
    void popAllAndPushAppState(AppState *state);
    // End of AppStateListener interface

 protected:
    void init(AppState *state);

    std::vector<AppState*> m_activeStateStack;
    std::vector<state_info> m_states;
    bool m_shutdown;
};

#endif
