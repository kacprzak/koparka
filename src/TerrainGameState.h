/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef TERRAIN_GAME_STATE_H
#define TERRAIN_GAME_STATE_H

#include "AppState.h"

#include "DotSceneLoader.h"

#include <OGRE/OgreSubEntity.h>
#include <OGRE/OgreMaterialManager.h>

enum QueryFlags {
    OGRE_HEAD_MASK = 1 << 0,
    CUBE_MASK      = 1 << 1
};

//------------------------------------------------------------------------------

class TerrainGameState : public AppState
{
 public:
    TerrainGameState();

    DECLARE_APPSTATE_CLASS(TerrainGameState)
    
    // AppState interface
    void enter();
    void exit();
    void update(double timeSinceLastFrame);
    bool pause();
    void resume();
    
    // OIS::KeyListener inteface
    bool keyPressed(const OIS::KeyEvent& event);
    bool keyReleased(const OIS::KeyEvent& event);

    // OIS::MouseListener inteface
    bool mouseMoved(const OIS::MouseEvent& event);
    bool mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID id);
    // End of OIS::MouseListener inteface
    
    void createScene();
    void moveCamera();
    void getInput();
    void buildGui();

    void onLeftPressed(const OIS::MouseEvent& event);
    void itemSelected(OgreBites::SelectMenu *menu);

 private:
    bool                   m_quit;

    Ogre::SceneNode        *m_ogreHeadNode;
    Ogre::Entity           *m_ogreHeadEntity;
    Ogre::MaterialPtr      m_ogreHeadMat;
    Ogre::MaterialPtr      m_ogreHeadMatHigh;

    OgreBites::ParamsPanel *m_detailsPanel;

    Ogre::Vector3          m_translateVector;
    Ogre::Real             m_moveSpeed;
    Ogre::Degree           m_rotateSpeed;
    float                  m_moveScale;
    Ogre::Degree           m_rotScale;

    Ogre::RaySceneQuery    *m_rsq;
    Ogre::SceneNode        *m_currentNode;
    Ogre::Entity           *m_currentEntity;
    bool                   m_leftMouseDown;
    bool                   m_rightMouseDown;
    bool                   m_settingsMode;
};

#endif
