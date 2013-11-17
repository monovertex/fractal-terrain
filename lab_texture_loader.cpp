//-------------------------------------------------------------------------------------------------
// Descriere: header in care sunt implementate functii pentru incarcare de texturi din fisiere BMP
//
// Nota:
//		loadTextureBMP - functia pe care o veti folosi pentru a incarca texturi.
//						 lucreaza cu functii OpenGL pentru a crea o textura (mai mult decat o imagine!)
//                       este functia care trebuie folosita
//---
//		_loadImageBMP  - incarca imaginea in memorie, independenta de OpenGL
//                       rol de functie de suport
//
// Nota2:
//		sunteti incurajati sa va scrieti parsele proprii pentru alte formaturi. Format sugerat: ppm, tga
//
// Autor: Lucian Petrescu
// Data: 28 Sep 2013
//-------------------------------------------------------------------------------------------------

#pragma once
#include "lab_texture_loader.h"

namespace lab{
    //definitie forward
    unsigned char* _loadBMPFile(const std::string &filename, unsigned int &width, unsigned int &height);

    //incarca o imagine BMP si creeaza cu ea o textura
    //aceasta este functia pe care o veti apela
    //returneaza id-ul texturii
    unsigned int loadTextureBMP(const std::string &filename){
        unsigned int width, height;
        unsigned char* data = _loadBMPFile(filename, width, height);

        //creeaza textura OpenGL
        unsigned int gl_texture_object;
        glGenTextures(1, &gl_texture_object);
        glBindTexture(GL_TEXTURE_2D, gl_texture_object);

        //filtrare
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        float maxAnisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

        //cand lucram cu texturi cu dimensiuni non multiple de 4 trebuie sa facem cititorul de randuri
        //ce incarca texturile in OpenGL sa lucreze cu memorie aliniata la 1 (default este la 4)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        //genereaza textura
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        //distruge array-ul din RAM
        delete data;

        //creaza ierarhia de mipmapuri
        glGenerateMipmap(GL_TEXTURE_2D);

        //returneaza obiectul textura
        return gl_texture_object;
    }

    //nu suporta compresie!
    //incarca un fisier BMP intr-un array unsigned char
    //returneaza un pointer la un array ce contine datele texturii si valori in argumentele trimise prin referinta width si height
    unsigned char* _loadBMPFile(const std::string &filename, unsigned int &width, unsigned int &height){
        //structuri pentru datele din header
        struct header{
            unsigned char type[2];
            int f_lenght;
            short rezerved1;
            short rezerved2;
            int offBits;
        };
        struct header_info{
            int size;
            int width;
            int height;
            short planes;
            short bitCount;
            int compresion;
            int sizeImage;
            int xPelsPerMeter;
            int yPelsPerMeter;
            int clrUsed;
            int clrImportant;
        };

        //citim din fisier
        std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
        if (!file.good()){
            std::cout << "Texture Loader: Nu am gasit fisierul bmp " << filename << " sau nu am drepturile sa il deschid!" << std::endl;
            width = 0;
            height = 0;
            return NULL;
        }
        std::cout << "Texture Loader: Incarc fisierul " << filename << std::endl;

        //citesc headere
        header h; header_info h_info;
        file.read((char*)&(h.type[0]), sizeof(char));
        file.read((char*)&(h.type[1]), sizeof(char));
        file.read((char*)&(h.f_lenght), sizeof(int));
        file.read((char*)&(h.rezerved1), sizeof(short));
        file.read((char*)&(h.rezerved2), sizeof(short));
        file.read((char*)&(h.offBits), sizeof(int));
        file.read((char*)&(h_info), sizeof(header_info));

        //aloc memorie (un pixel are 3 componente R, G, B)
        unsigned char *data = new unsigned char[h_info.width*h_info.height * 3];

        // verific daca exista grupuri de 4 octeti pe linie
        long padd = 0;
        if ((h_info.width * 3) % 4 != 0) padd = 4 - (h_info.width * 3) % 4;

        //save height &width
        width = h_info.width;
        height = h_info.height;

        long pointer;
        unsigned char r, g, b;
        //citesc matricea
        for (unsigned int i = 0; i < height; i++)
        {
            for (unsigned int j = 0; j < width; j++)
            {
                file.read((char*)&b, 1);	//in bmp ordinea in pixel este b,g,r (specific windows)
                file.read((char*)&g, 1);
                file.read((char*)&r, 1);

                pointer = (i*width + j) * 3;
                data[pointer] = r;
                data[pointer + 1] = g;
                data[pointer + 2] = b;
            }

            file.seekg(padd, std::ios_base::cur);
        }
        file.close();

        //returneaza datele
        return data;
    }
}