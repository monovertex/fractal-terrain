//-------------------------------------------------------------------------------------------------
// Descriere:
//		fisier header in care e implementat un handler pentru freeglut, care ofera:
//		- o fereastra
//		- control asupra inputului(mouse, keyboard)
//		- un context de OpenGL fara de care nu am putea lucra cu OpenGL
//
// OBS:
//		FREEGLUT NU ESTE OPENGL, exista multe alte sisteme de fereastra+input+context ce pot
//      fi folosite cu OpenGL. (ex: glfw, sdl, fltk, glow, cod nativ , etc)
//
// Note:
//      Cum freeglut utilizeaza functii c este imposibil sa ii oferim metode de clasa fara
//		artificii de design. Cum scopul codului este acela de a maximiza claritatea si de a fi
//		cat se poate de simplu vom folosi functii statice.
//		Pentru a pastra un nivel minim de organizare a codului vom folosi un namespace doar pentru glut.
//
// Note2:
//		daca o functie sau o variabila incepe cu underscore (_) atunci (f probabil) ca nu trebuie sa
//      lucrati direct cu ea
//
// Autor: Lucian Petrescu
// Data: 28 Sep 2013
//-------------------------------------------------------------------------------------------------

#pragma once

#include "lab_glut_support.h"
#include <iostream>

namespace lab{
    namespace glut{
        void _idleCallback();

        void _displayCallback();

        void _reshapeCallback(int width, int height);

        //defintie forward pt aceasta functie, e implementata la linia 193
        void close();

        void _keyboardDownCallback(unsigned char key_pressed, int mouse_x, int mouse_y);

        void _keyboardUpCallback(unsigned char key_released, int mouse_x, int mouse_y);

        void _specialDownCallback(int key_pressed, int mouse_x, int mouse_y);

        void _specialUpCallback(int key_released, int mouse_x, int mouse_y);

        void _motionCallback(int mouse_x, int mouse_y);

        void _passiveMotionCallback(int mouse_x, int mouse_y);

        void _mouseCallback(int button, int state, int mouse_x, int mouse_y);

        void _mouseWheelCallback(int wheel, int direction, int mouse_x, int mouse_y);

        //-----------------------------------------------------------------------------------------
        //initializare glut, functia necesita 3 parametri
        // -window, de tip WindowInfo, descrie fereastra ce va fi oferita
        // -context, de tip ContextInfo, ce descrie contextul pe care il cerem de la driver
        // -framebuffer, de tip FramebufferInfo, ce descrie formatul datelor din framebufferul oferit de glut
        void init(const WindowInfo &window, const ContextInfo &context, const FramebufferInfo &framebuffer);

        //-----------------------------------------------------------------------------------------
        // GLUT incepe sa ruleze (cadru dupa cadru)
        void run();
        //functia aceasta o apelati cand doriti sa inchideti glut
        void close();

        //-----------------------------------------------------------------------------------------
        // detalii despre framebuffer
        const FramebufferInfo& getInitialFramebufferInformation();
        //detalii despre fereastra
        const WindowInfo& getInitialWindowInformation();
        //detalii despre context
        const ContextInfo& getContextInfo();

        //-----------------------------------------------------------------------------------------
        //fullscreen
        void enterFullscreen();
        void exitFullscreen();

        //-----------------------------------------------------------------------------------------
        //listener
        void setListener(WindowListener* listener);
        WindowListener* getListener();
    }
}
