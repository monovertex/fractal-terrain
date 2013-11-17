//-------------------------------------------------------------------------------------------------
// Descriere: header in care sunt definite niste functii de desenare rapida
// Nota:
//		wrappere peste functii din glut, cand vom invata atribute si vbo-uri si ibo-uri(lab 2 shadere)
//		vor deveni utile doar pentru prototipari rapide. Aceste functii NU sunt eficiente!
//
// Nota2:
//		singura exceptie in care folosim obiecte din glut in afara namespaceului lab::glut
//		in mod normal acestea ar fi implementate manual cu vbo-uri si ibo-uri (lab 2 shadere)
//
// Nota3:
//		exista alte multe functii de glut pentru desenare printr-o singura comanda.
//
// Autor: Lucian Petrescu
// Data: 28 Sep 2013
//-------------------------------------------------------------------------------------------------

#pragma once
#include "dependencies\freeglut\freeglut.h"

namespace lab{
    //sfera
    void drawSolidSphere(float radius, int slices, int stacks);
    void drawWireSphere(float radius, int slices, int stacks);

    //cub
    void drawSolidCube(float size);
    void drawWireCube(float size);

    //con
    void drawSolidCone(float base_radius, float height, int slices, int stacks);
    void drawWireCone(float base_radius, float height, int slices, int stacks);

    //torus
    void drawSolidTorus(float inner_radius, float outer_radius, int nsides, int rings);
    void drawWireTorus(float inner_radius, float outer_radius, int nsides, int rings);

    //dodecaedru
    void drawSolidDodecahedron();
    void drawWireDodecahedron();

    //octaedru
    void drawSolidOctahedron();
    void drawWireOctahedron();

    //tetraedru
    void drawSolidTetrahedron();
    void drawWireTetrahedron();

    //icosaedru
    void drawSolidIcosahedron();
    void drawWireIcosahedron();

    //ceainic (poveste: https://en.wikipedia.org/wiki/Utah_teapot)
    void drawSolidTeapot(float size);
    void drawWireTeapot(float size);
}
