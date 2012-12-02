/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "AppState.h"

class MenuState : public AppState
{
 public:
    MenuState();

    DECLARE_APPSTATE_CLASS(MenuState)
    
    // AppState interface
    void enter();
    void exit();
    void update(double timeSinceLastFrame);
    
    // OIS::KeyListener inteface
    bool keyPressed(const OIS::KeyEvent& event);
    bool keyReleased(const OIS::KeyEvent& event);

    // OIS::MouseListener inteface
    bool mouseMoved(const OIS::MouseEvent& event);
    bool mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID id);
    // End of OIS::MouseListener inteface
    
    void createScene();
    void buttonHit(OgreBites::Button *button);

 private:
    bool m_quit;
};

#endif
