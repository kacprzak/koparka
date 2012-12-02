/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef DEMO_APP_H
#define DEMO_APP_H

class AppStateManager;

class DemoApp
{
 public:
    DemoApp();
    ~DemoApp();

    void startDemo();

 private:
    AppStateManager *m_appStateManager;
};

#endif
