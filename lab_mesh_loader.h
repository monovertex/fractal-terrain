//-------------------------------------------------------------------------------------------------
// Descriere: todo
//
// Contine:
//		todo
//
//// Nota2:
//		sunteti incurajati sa va scrieti parsele proprii pentru alte formaturi. Format sugerat: ply,off
//
// Autor: Lucian Petrescu
// Data: 28 Sep 2013
//-------------------------------------------------------------------------------------------------

#pragma once

#include "dependencies\glew\glew.h"
#include "dependencies\glm\glm.hpp"
#include "dependencies\glm\gtc\type_ptr.hpp"
#include "dependencies\glm\gtc\matrix_transform.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

namespace lab{
    //care este formatul unui vertex?
    struct VertexFormat{
        float position_x, position_y, position_z;				//pozitia unui vertex (x,y,z)
        float normal_x, normal_y, normal_z;						//vom invata ulterior, nu este folosit in acest lab
        float texcoord_x, texcoord_y;							//vom invata ulterior, nu este folosit in acest lab
        VertexFormat();
        VertexFormat(float px, float py, float pz);
        VertexFormat(float px, float py, float pz, float nx, float ny, float nz);
        VertexFormat(float px, float py, float pz, float tx, float ty);
        VertexFormat(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty);
        VertexFormat operator=(const VertexFormat &rhs);
    };

    //incarca un fisier de tip Obj (fara NURBS, fara materiale)
    //returneaza in argumentele trimise prin referinta id-ul OpenGL pentru vao(Vertex Array Object), pentru vbo(Vertex Buffer Object) si pentru ibo(Index Buffer Object)
    void loadObj(const std::string &filename, unsigned int &vao, unsigned int& vbo, unsigned int &ibo, unsigned int &num_indices);

    //-------------------------------------------------------------------------------------------------
    //urmeaza cod de parsare...

    //helper funcs
    float _stringToFloat(const std::string &source);
    //transforms a string to an int
    unsigned int _stringToUint(const std::string &source);
    //transforms a string to an int
    int _stringToInt(const std::string &source);
    //writes the tokens of the source string into tokens
    void _stringTokenize(const std::string &source, std::vector<std::string> &tokens);
    //variant for faces
    void _faceTokenize(const std::string &source, std::vector<std::string> &tokens);

    //incarca doar geometrie dintr-un fisier obj (nu incarca high order surfaces, materiale, coordonate extra, linii)
    // Format: http://paulbourke.net/dataformats/obj/
    //nu calculeaza normale sau coordonate de textura sau tangente, performanta neoptimala dar usor de citit (relativ la alte parsere..)
    //considera geometria ca pe un singur obiect, deci nu tine cont de grupuri sau de smoothing
    //daca apar probleme la incarcare fisier(?) incarcati mesha originala in meshlab(free!), salvati si folositi varianta noua.
    void _loadObjFile(const std::string &filename, std::vector<VertexFormat> &vertices, std::vector<unsigned int> &indices);
}