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
#include "lab_shader_loader.h"

namespace lab{
    //creaza program shader din vertex shader si fragment shader
    //returneaza id-ul programului shader OpenGL
    unsigned int loadShader(const std::string &vertex_shader_file, const std::string &fragment_shader_file){
        //creaza o lista cu toate shaderele
        std::vector<unsigned int> shaders;
        shaders.push_back(_createShader(vertex_shader_file, GL_VERTEX_SHADER));
        shaders.push_back(_createShader(fragment_shader_file, GL_FRAGMENT_SHADER));

        std::cout << "Shader Loader : incarc shaderul compus din " << std::endl;
        std::cout << "\tvertex shader = " << vertex_shader_file << std::endl;
        std::cout << "\tframgnet shader = " << fragment_shader_file << std::endl;
        return _createProgram(shaders);
    }
    //creeaza program shader din vertex shader, geometry shader, fragment shader
    //returneaza id-ul programului shader OpenGL
    unsigned int loadShader(const std::string &vertex_shader_file, const std::string &geometry_shader_file, const std::string &fragment_shader_file){
        //creaza o lista cu toate shaderele
        std::vector<unsigned int> shaders;
        shaders.push_back(_createShader(vertex_shader_file, GL_VERTEX_SHADER));
        shaders.push_back(_createShader(geometry_shader_file, GL_GEOMETRY_SHADER));
        shaders.push_back(_createShader(fragment_shader_file, GL_FRAGMENT_SHADER));

        std::cout << "Shader Loader : incarc shaderul compus din " << std::endl;
        std::cout << "\tvertex shader = " << vertex_shader_file << std::endl;
        std::cout << "\tgeometry shader = " << geometry_shader_file << std::endl;
        std::cout << "\tframgnet shader = " << fragment_shader_file << std::endl;
        return _createProgram(shaders);
    }

    //creaza un obiect de tip shader
    unsigned int _createShader(const std::string &shader_file, GLenum shader_type){
        //incarca fisierul intr-un string
        std::string shader_code;
        std::ifstream file(shader_file.c_str(), std::ios::in);
        if (!file.good()){
            std::cout << "Shader Loader: Nu am gasit fisierul shader " << shader_file << " sau nu am drepturile sa il deschid!" << std::endl;
            std::terminate();
        }
        file.seekg(0, std::ios::end);
        shader_code.resize((unsigned int)file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&shader_code[0], shader_code.size());
        file.close();

        int info_log_length = 0, compile_result = 0;
        unsigned int gl_shader_object;

        //construieste un obiect de tip shader din codul incarcat
        gl_shader_object = glCreateShader(shader_type);
        const char *shader_code_ptr = shader_code.c_str();
        const int shader_code_size = shader_code.size();
        glShaderSource(gl_shader_object, 1, &shader_code_ptr, &shader_code_size);
        glCompileShader(gl_shader_object);
        glGetShaderiv(gl_shader_object, GL_COMPILE_STATUS, &compile_result);

        //daca exista erori output la consola
        if (compile_result == GL_FALSE){
            std::string str_shader_type = "";
            if (shader_type == GL_VERTEX_SHADER) str_shader_type = "vertex shader";
            if (shader_type == GL_TESS_CONTROL_SHADER) str_shader_type = "tess control shader";
            if (shader_type == GL_TESS_EVALUATION_SHADER) str_shader_type = "tess evaluation shader";
            if (shader_type == GL_GEOMETRY_SHADER) str_shader_type = "geometry shader";
            if (shader_type == GL_FRAGMENT_SHADER) str_shader_type = "fragment shader";
            if (shader_type == GL_COMPUTE_SHADER) str_shader_type = "compute shader";

            glGetShaderiv(gl_shader_object, GL_INFO_LOG_LENGTH, &info_log_length);
            std::vector<char> shader_log(info_log_length);
            glGetShaderInfoLog(gl_shader_object, info_log_length, NULL, &shader_log[0]);
            std::cout << "Shader Loader: EROARE DE COMPILARE pentru " << str_shader_type << std::endl << &shader_log[0] << std::endl;
            return 0;
        }

        return gl_shader_object;
    }
    // creaza un program de tip shader
    unsigned int _createProgram(const std::vector<unsigned int> &shader_objects){
        int info_log_length = 0, link_result = 0;

        //build OpenGL program object and link all the OpenGL shader objects
        unsigned int gl_program_object = glCreateProgram();
        for (std::vector<unsigned int>::const_iterator it = shader_objects.begin(); it != shader_objects.end(); it++) glAttachShader(gl_program_object, (*it));
        glLinkProgram(gl_program_object);
        glGetProgramiv(gl_program_object, GL_LINK_STATUS, &link_result);

        //if we get link errors log them
        if (link_result == GL_FALSE){
            glGetProgramiv(gl_program_object, GL_INFO_LOG_LENGTH, &info_log_length);
            std::vector<char> program_log(info_log_length);
            glGetProgramInfoLog(gl_program_object, info_log_length, NULL, &program_log[0]);
            std::cout << "Shader Loader : EROARE DE LINKARE" << std::endl << &program_log[0] << std::endl;
            return 0;
        }

        //delete the shader objects because we do not need them any more (this should be re-written if usage of separate shader objects is intended!)
        for (std::vector<unsigned int>::const_iterator it = shader_objects.begin(); it != shader_objects.end(); it++) glDeleteShader((*it));

        return gl_program_object;
    }
}