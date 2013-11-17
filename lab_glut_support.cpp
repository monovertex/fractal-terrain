//-------------------------------------------------------------------------------------------------
// Descriere: header in care sunt definite structuri folosite de lab_glut sau utilizate pentru a
// comunica cu lab_glut
// Nota:
//		WindowInfo      - stocheaza informatii despre fereastra (inaltime, lungime, etc)
//		FramebufferInfo - stocheaza inforatii despre framebuffer
//		ContextInfo     - stocheaza informatii despre contextul opengl CERUT.
//		WindowListener  - clasa din care trebuie sa derivam pentru a lucra cu glut
//
// Nota2:
//		tot ce apartine de glut se afla intr-un namespace propriu
//
// Autor: Lucian Petrescu
// Data: 28 Sep 2013
//-------------------------------------------------------------------------------------------------

#pragma once
#include "lab_glut_support.h"

namespace lab{
    namespace glut{
        //-------------------------------------------------------------------------------------------------
        // Informatii despre fereastra
        WindowInfo::WindowInfo(){
            name = "nume default fereastra";
            width = 800; height = 600; start_position_x = start_position_y = 100;
            is_reshapable = true;
        }
        WindowInfo::WindowInfo(std::string name, int width, int height, int start_position_x, int start_position_y, bool is_reshapable){
            this->name = name;
            this->width = width;
            this->height = height;
            this->start_position_x = start_position_x;
            this->start_position_y = start_position_y;
            this->is_reshapable = is_reshapable;
        }
        void WindowInfo::operator =(const WindowInfo& info){
            name = info.name;
            width = info.width;
            height = info.height;
            start_position_x = info.start_position_x;
            start_position_y = info.start_position_y;
            is_reshapable = info.is_reshapable;
        }

        //-------------------------------------------------------------------------------------------------
        // informatii despre framebuffer
        FramebufferInfo::FramebufferInfo(){
            flags = GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH;
            msaa = false;
        }
        FramebufferInfo::FramebufferInfo(bool color, bool depth, bool stencil, bool msaa){
            flags = GLUT_DOUBLE;					//tot timpul folosim double buffering!
            if (color) flags |= GLUT_RGBA | GLUT_ALPHA;
            if (depth) flags |= GLUT_DEPTH;
            if (stencil) flags |= GLUT_STENCIL;
            if (msaa) flags |= GLUT_MULTISAMPLE;
            this->msaa = msaa;
        }
        void FramebufferInfo::operator = (const FramebufferInfo& info) {
            flags = info.flags;
            msaa = info.msaa;
        }

        //-------------------------------------------------------------------------------------------------
        //informatii despre contextul OpenGL
        ContextInfo::ContextInfo(){
            major_version = 3;						//cel mai nou context e 4.4
            minor_version = 3;						//dar nu toate driverele sustin inca functionalitatea
            core = true;							//forteaza doar functiile core pentru contextul cerut.
        }
        ContextInfo::ContextInfo(int major_version, int minor_version, bool core){
            this->major_version = major_version;
            this->minor_version = minor_version;
            this->core = core;
        }
        void ContextInfo::operator=(const ContextInfo &info){
            major_version = info.major_version;
            minor_version = info.minor_version;
            core = info.core;
        }

        //---------------------------------------------------------------------------------------------------
        //clasa din care trebuie derivat pentru a putea lucra cu glut
        //functioneza ca un listener pe lab::glut

        WindowListener::WindowListener(){}
        WindowListener::~WindowListener(){}

        //------------------- functii ce tin cadrul curent
        //functie chemata inainte de inceperea cadrului curent (adica inainte de a incepe procesul de afisare)
        void WindowListener::notifyBeginFrame(){}
        //functie de afisare, chemata inainte afisarea efectiva (swapBuffers)
        void WindowListener::notifyDisplayFrame(){}
        //functie chemata dupa sfarsirea procesului de afisare pe CPU
        void WindowListener::notifyEndFrame(){}

        //------------------- reshape - se apealeaza atunci cand ecranul este
        void WindowListener::notifyReshape(int width, int height, int previos_width, int previous_height){}

        //------------------- functii de input
        //functie chemata cand e apasata o tasta
        void WindowListener::notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y){}
        //functie chemata cand se termina apasarea unei taste
        void WindowListener::notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y){}
        //functie chemata cand o tasta speciala e apasata (up down, left right, F1-12, etc)
        void WindowListener::notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y){}
        //functie chemata cand ose termina apsarea unei taste speciale (up down, left right, F1-12, etc)
        void WindowListener::notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y){}
        //functie chemata cand se face mouse drag
        void WindowListener::notifyMouseDrag(int mouse_x, int mouse_y){}
        //functie chemata cand mouse-ul se misca
        void WindowListener::notifyMouseMove(int mouse_x, int mouse_y){}
        //functie chemata cand un button de mouse e apasat
        void WindowListener::notifyMouseClick(int button, int state, int mouse_x, int mouse_y){}
        //functie chemata cand se face scroll cu mouse-ul.
        void WindowListener::notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y){}
    }
}