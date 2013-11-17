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

#include "lab_mesh_loader.h"

namespace lab{
    //care este formatul unui vertex?
    VertexFormat::VertexFormat(){
        position_x = position_y = position_z = 0;
        normal_x = normal_y = normal_z = 0;
        texcoord_x = texcoord_y = 0;
    }
    VertexFormat::VertexFormat(float px, float py, float pz){
        position_x = px;		position_y = py;		position_z = pz;
        normal_x = normal_y = normal_z = 0;
        texcoord_x = texcoord_y = 0;
    }
    VertexFormat::VertexFormat(float px, float py, float pz, float nx, float ny, float nz){
        position_x = px;		position_y = py;		position_z = pz;
        normal_x = nx;		normal_y = ny;		normal_z = nz;
        texcoord_x = texcoord_y = 0;
    }
    VertexFormat::VertexFormat(float px, float py, float pz, float tx, float ty){
        position_x = px;		position_y = py;		position_z = pz;
        texcoord_x = tx;		texcoord_y = ty;
        normal_x = normal_y = normal_z = 0;
    }
    VertexFormat::VertexFormat(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty){
        position_x = px;		position_y = py;		position_z = pz;
        normal_x = nx;		normal_y = ny;		normal_z = nz;
        texcoord_x = tx;		texcoord_y = ty;
    }
    VertexFormat VertexFormat::operator=(const VertexFormat &rhs){
        position_x = rhs.position_x;	position_y = rhs.position_y;	position_z = rhs.position_z;
        normal_x = rhs.normal_x;		normal_y = rhs.normal_y;		normal_z = rhs.normal_z;
        texcoord_x = rhs.texcoord_x;	texcoord_y = rhs.texcoord_y;
        return (*this);
    }

    //incarca un fisier de tip Obj (fara NURBS, fara materiale)
    //returneaza in argumentele trimise prin referinta id-ul OpenGL pentru vao(Vertex Array Object), pentru vbo(Vertex Buffer Object) si pentru ibo(Index Buffer Object)
    void loadObj(const std::string &filename, unsigned int &vao, unsigned int& vbo, unsigned int &ibo, unsigned int &num_indices){
        //incarca vertecsii si indecsii din fisier
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;
        _loadObjFile(filename, vertices, indices);

        std::cout << "Mesh Loader : am incarcat fisierul " << filename << std::endl;

        //creeaza obiectele OpenGL necesare desenarii
        unsigned int gl_vertex_array_object, gl_vertex_buffer_object, gl_index_buffer_object;

        //vertex array object -> un obiect ce reprezinta un container pentru starea de desenare
        glGenVertexArrays(1, &gl_vertex_array_object);
        glBindVertexArray(gl_vertex_array_object);

        //vertex buffer object -> un obiect in care tinem vertecsii
        glGenBuffers(1, &gl_vertex_buffer_object);
        glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_object);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

        //index buffer object -> un obiect in care tinem indecsii
        glGenBuffers(1, &gl_index_buffer_object);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_index_buffer_object);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        //legatura intre atributele vertecsilor si pipeline, datele noastre sunt INTERLEAVED.
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);						//trimite pozitii pe pipe 0
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(float)* 3));		//trimite normale pe pipe 1
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(float)* 3));	//trimite texcoords pe pipe 2

        vao = gl_vertex_array_object;
        vbo = gl_vertex_buffer_object;
        ibo = gl_index_buffer_object;
        num_indices = indices.size();
    }

    //-------------------------------------------------------------------------------------------------
    //urmeaza cod de parsare...

    //helper funcs
    float _stringToFloat(const std::string &source){
        std::stringstream ss(source.c_str());
        float result;
        ss >> result;
        return result;
    }
    //transforms a string to an int
    unsigned int _stringToUint(const std::string &source){
        std::stringstream ss(source.c_str());
        unsigned int result;
        ss >> result;
        return result;
    }
    //transforms a string to an int
    int _stringToInt(const std::string &source){
        std::stringstream ss(source.c_str());
        int result;
        ss >> result;
        return result;
    }
    //writes the tokens of the source string into tokens
    void _stringTokenize(const std::string &source, std::vector<std::string> &tokens){
        tokens.clear();
        std::string aux = source;
        for (unsigned int i = 0; i<aux.size(); i++) if (aux[i] == '\t' || aux[i] == '\n') aux[i] = ' ';
        std::stringstream ss(aux, std::ios::in);
        while (ss.good()){
            std::string s;
            ss >> s;
            if (s.size()>0) tokens.push_back(s);
        }
    }
    //variant for faces
    void _faceTokenize(const std::string &source, std::vector<std::string> &tokens){
        std::string aux = source;
        for (unsigned int i = 0; i < aux.size(); i++) if (aux[i] == '\\' || aux[i] == '/') aux[i] = ' ';
        _stringTokenize(aux, tokens);
    }

    //incarca doar geometrie dintr-un fisier obj (nu incarca high order surfaces, materiale, coordonate extra, linii)
    // Format: http://paulbourke.net/dataformats/obj/
    //nu calculeaza normale sau coordonate de textura sau tangente, performanta neoptimala dar usor de citit (relativ la alte parsere..)
    //considera geometria ca pe un singur obiect, deci nu tine cont de grupuri sau de smoothing
    //daca apar probleme la incarcare fisier(?) incarcati mesha originala in meshlab(free!), salvati si folositi varianta noua.
    void _loadObjFile(const std::string &filename, std::vector<VertexFormat> &vertices, std::vector<unsigned int> &indices){
        //citim din fisier
        std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
        if (!file.good()){
            std::cout << "Mesh Loader: Nu am gasit fisierul obj " << filename << " sau nu am drepturile sa il deschid!" << std::endl;
            std::terminate();
        }

        std::string line;
        std::vector<std::string> tokens, facetokens;
        std::vector<glm::vec3> positions;		positions.reserve(1000);
        std::vector<glm::vec3> normals;		normals.reserve(1000);
        std::vector<glm::vec2> texcoords;		texcoords.reserve(1000);
        while (std::getline(file, line)){
            //tokenizeaza linie citita
            _stringTokenize(line, tokens);

            //daca nu am nimic merg mai departe
            if (tokens.size() == 0) continue;

            //daca am un comentariu merg mai departe
            if (tokens.size() > 0 && tokens[0].at(0) == '#') continue;

            //daca am un vertex
            if (tokens.size() > 3 && tokens[0] == "v") positions.push_back(glm::vec3(_stringToFloat(tokens[1]), _stringToFloat(tokens[2]), _stringToFloat(tokens[3])));

            //daca am o normala
            if (tokens.size() > 3 && tokens[0] == "vn") normals.push_back(glm::vec3(_stringToFloat(tokens[1]), _stringToFloat(tokens[2]), _stringToFloat(tokens[3])));

            //daca am un texcoord
            if (tokens.size() > 2 && tokens[0] == "vt") texcoords.push_back(glm::vec2(_stringToFloat(tokens[1]), _stringToFloat(tokens[2])));

            //daca am o fata (f+ minim 3 indecsi)
            if (tokens.size() >= 4 && tokens[0] == "f"){
                //foloseste primul vertex al fetei pentru a determina formatul fetei (v v/t v//n v/t/n) = (1 2 3 4)
                unsigned int face_format = 0;
                if (tokens[1].find("//") != std::string::npos) face_format = 3;
                _faceTokenize(tokens[1], facetokens);
                if (facetokens.size() == 3) face_format = 4; // vertecsi/texcoords/normale
                else{
                    if (facetokens.size() == 2){
                        if (face_format != 3) face_format = 2;	//daca nu am vertecsi/normale am vertecsi/texcoords
                    }
                    else{
                        face_format = 1; //doar vertecsi
                    }
                }

                //primul index din acest poligon
                unsigned int index_of_first_vertex_of_face = -1;

                for (unsigned int num_token = 1; num_token<tokens.size(); num_token++){
                    if (tokens[num_token].at(0) == '#') break;					//comment dupa fata
                    _faceTokenize(tokens[num_token], facetokens);
                    if (face_format == 1){
                        //doar pozitie
                        int p_index = _stringToInt(facetokens[0]);
                        if (p_index>0) p_index -= 1;								//obj has 1...n indices
                        else p_index = positions.size() + p_index;				//index negativ

                        vertices.push_back(VertexFormat(positions[p_index].x, positions[p_index].y, positions[p_index].z));
                    }
                    else if (face_format == 2){
                        // pozitie si texcoord
                        int p_index = _stringToInt(facetokens[0]);
                        if (p_index > 0) p_index -= 1;								//obj has 1...n indices
                        else p_index = positions.size() + p_index;				//index negativ

                        int t_index = _stringToInt(facetokens[1]);
                        if (t_index > 0) t_index -= 1;								//obj has 1...n indices
                        else t_index = texcoords.size() + t_index;				//index negativ

                        vertices.push_back(VertexFormat(positions[p_index].x, positions[p_index].y, positions[p_index].z, texcoords[t_index].x, texcoords[t_index].y));
                    }
                    else if (face_format == 3){
                        //pozitie si normala
                        int p_index = _stringToInt(facetokens[0]);
                        if (p_index > 0) p_index -= 1;								//obj has 1...n indices
                        else p_index = positions.size() + p_index;				//index negativ

                        int n_index = _stringToInt(facetokens[1]);
                        if (n_index > 0) n_index -= 1;								//obj has 1...n indices
                        else n_index = normals.size() + n_index;					//index negativ

                        vertices.push_back(VertexFormat(positions[p_index].x, positions[p_index].y, positions[p_index].z, normals[n_index].x, normals[n_index].y, normals[n_index].z));
                    }
                    else{
                        //pozitie normala si texcoord
                        int p_index = _stringToInt(facetokens[0]);
                        if (p_index > 0) p_index -= 1;								//obj has 1...n indices
                        else p_index = positions.size() + p_index;				//index negativ

                        int t_index = _stringToInt(facetokens[1]);
                        if (t_index > 0) t_index -= 1;								//obj has 1...n indices
                        else t_index = normals.size() + t_index;					//index negativ

                        int n_index = _stringToInt(facetokens[2]);
                        if (n_index > 0) n_index -= 1;								//obj has 1...n indices
                        else n_index = normals.size() + n_index;					//index negativ

                        vertices.push_back(VertexFormat(positions[p_index].x, positions[p_index].y, positions[p_index].z, normals[n_index].x, normals[n_index].y, normals[n_index].z, texcoords[t_index].x, texcoords[t_index].y));
                    }

                    //adauga si indecsii
                    if (num_token < 4){
                        if (num_token == 1) index_of_first_vertex_of_face = vertices.size() - 1;
                        //doar triunghiuri f 0 1 2 3 (4 indecsi, primul e ocupat de f)
                        indices.push_back(vertices.size() - 1);
                    }
                    else{
                        //polygon => triunghi cu ultimul predecesor vertexului nou adaugat si 0 relativ la vertecsi poligon(independent clockwise)
                        indices.push_back(index_of_first_vertex_of_face);
                        indices.push_back(vertices.size() - 2);
                        indices.push_back(vertices.size() - 1);
                    }
                }//end for
            }//end face
        }//end while
    }
}