//-------------------------------------------------------------------------------------------------
// Descriere: header in care sunt implementate functii pentru incarcare de texturi din fisiere BMP
//
// Note:
//		loadShader		- functia pe care o veti folosi pentru a incarca un shader din mai multe fisiere
//---
//		_createShader	- functie ce creaza un obiect shader dintr-un fisier
//		_createProgram	- functie ce creaza un program opengl din mai multe obiecte shader
//
// Nota2:
//		un program shader include mai multe obiecte de tip shader, pentru fiecare din etapele
//      benzii de programare dinamice.
//
// Autor: Lucian Petrescu
// Data: 28 Sep 2013
//-------------------------------------------------------------------------------------------------

#pragma once

#include "dependencies\glew\glew.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace lab{
    //creaza program shader din vertex shader si fragment shader
    //returneaza id-ul programului shader OpenGL
    unsigned int loadShader(const std::string &vertex_shader_file, const std::string &fragment_shader_file);
    //creeaza program shader din vertex shader, geometry shader, fragment shader
    //returneaza id-ul programului shader OpenGL
    unsigned int loadShader(const std::string &vertex_shader_file, const std::string &geometry_shader_file, const std::string &fragment_shader_file);

    //creaza un obiect de tip shader
    unsigned int _createShader(const std::string &shader_file, GLenum shader_type);
    // creaza un program de tip shader
    unsigned int _createProgram(const std::vector<unsigned int> &shader_objects);
}
