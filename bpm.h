#ifndef bmp_h
#define bmp_h
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
class BMP{
    public:
    BMP(){};
     GLuint getBMP(const char *path){
        unsigned char header[54];
        unsigned int dataPos;
        unsigned int width, height;
        unsigned int imageSize, wAlpha;
        int pixel;

        FILE * file = fopen(path,"rb");
        if(!file){
            printf("UNABLE TO OPEN %s\n", path);
            return 0;
        }

        if(fread(header,1,54,file) != 54){
            printf("NOT A BMP FILE\n");
            return 0;
        }
        if(header[0] != 'B' || header[1] != 'M'){
            printf("NOT A BMP FILE\n");
            return 0;
        }
        // read image header imformation
        dataPos = *(int*)&(header[0x0A]);
        imageSize = *(int*)&(header[0x22]);
        width = *(int*)&(header[0x12]);
        height = *(int*)&(header[0x16]);
        
        // if the image header is corupted somehow
        if(imageSize == 0){
            imageSize = width * height * 3;
        }
        if(dataPos == 0)
            dataPos = 54;
        // get image data
        pixel = width * height;
        wAlpha = width * height * 4;
        std::cout<<"Getting image data "<<imageSize<<"\n"<<wAlpha<<"\n";
        unsigned char data[imageSize];
        std::cout<<"after height\n";
        unsigned char data_A[wAlpha];
        fread(data, 1, imageSize, file);
        fclose(file);

        // apply apha channel
        for(int i=0; i< pixel; i++){
            data_A[i*4] = data[i*3];
            data_A[i*4+1] = data[i*3+1];
            data_A[i*4+2] = data[i*3+2];
            if(data[i*3]== 9 and data[i*3+1]== 249 and data[i*3+2]== 31){
                data_A[i*4+3] = 0;
            }else
                data_A[i*4+3] = 250;
        }

        // format image to gl format
        GLuint textureID;
        glGenTextures(1, &textureID);

        glBindTexture(GL_TEXTURE_2D, textureID);

        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data_A);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

        return textureID;
    }
};

#endif
