/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/ForeignWindowControl>
#include <bb/cascades/Container>
#include <bb/cascades/AbsoluteLayout>
#include <bb/cascades/CustomControl>
#include <bb/cascades/Button>
#include <bb/cascades/Page>

#include "pthread.h"
#include "gl_main.h"

using namespace bb::cascades;

/**
 * Function to call for pThread.
 * Sends window info to gl_main.
 * More -> gl_main.c/.h
 */
void test(){
    qDebug()<< "test function called from thread";
}
void* glThread(void* arg)
{
    ForeignWindowControl* inst = (ForeignWindowControl*)arg;
    qDebug() << "starting main";
    qDebug() << "Window ID passed to thread:  " << inst->windowId().toStdString().c_str();
    qDebug() << "Length():  " << inst->windowId().toStdString().length();
    qDebug() << "window group ID:  " << inst->windowGroup().toStdString().c_str();
    //qDebug() << "window group ID length:  " << inst->windowGroup().toStdString().length();
    //while(true){

    int err = gl_main(inst->windowId().toStdString().c_str(),
            inst->windowId().toStdString().length(),
            inst->windowGroup().toStdString().c_str());
    //}

    //test();
    //usleep(10000);
    qDebug() << "gl_main() concluded!" ;
    pthread_exit((void*)0);
}

ApplicationUI::ApplicationUI() :
        QObject()
{
    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(res);

    // initial load
    onSystemLanguageChanged();

    // create our foreign window
    mGlWindow = ForeignWindowControl::create();
    mGlWindow->setWindowId(QString("glWindow"));

    //create button
    mButton = Button::create("start");

    //connect signals and slots
    QObject::connect(mButton,
                     SIGNAL(clicked()), this, SLOT(onButtonClicked()));

    QObject::connect(mGlWindow,
                     SIGNAL(windowAttached(screen_window_t,
                                           const QString &, const QString &)),
                     this,
                     SLOT(onWindowAttached(screen_window_t,
                          const QString &,const QString &)));

    //create our container
    Container* container = Container::create()
        .layout(AbsoluteLayout::create())
        .add(mGlWindow)
        .add(mButton);

    Page* mainPage = new Page();
    mainPage->setContent(container);

    //set scene to page.
    Application::instance()->setScene(mainPage);
}


void ApplicationUI::onWindowAttached(screen_window_t handle,
                           const QString &group,
                           const QString &id)
{
    qDebug() << "onWindowAttached: " << handle << ", " << group << ", " << id;
    screen_window_t window = (screen_window_t)handle;
    // make window visible and position it behind cascades
    int i = 1;

    //set screen to visible
    screen_set_window_property_iv(window, SCREEN_PROPERTY_VISIBLE, &i);
    i = -1;

    //set depth to below cascades
    screen_set_window_property_iv(window, SCREEN_PROPERTY_ZORDER, &i);
}

void ApplicationUI::onButtonClicked()
{
    qDebug() << "onButtonClicked";
    // only let the button be clicked once
    mButton->setEnabled(false);
    // spawn a thread to do the opengl stuff
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    pthread_attr_setdetachstate(
       &attr, PTHREAD_CREATE_DETACHED );
    pthread_create(&mTid, &attr, &glThread, (void *)mGlWindow);

}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("CascadesProject_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}
