#include <stdio.h>
#define _OPEN_SYS_ITOA_EXT
#include <string.h>
#include <stdlib.h>
#include "lib_ppm.h"

unsigned int decToBinary(int n)
{
    // Tamanho do número em bits (12 bits no caso)
    int tamanho = 12;
    unsigned int binario = 0;

    for (int i = tamanho - 1; i >= 0; i--)
    {
        // Verifica o bit no i-ésimo posição
        int bit = (n >> i) & 1;

        // Define o bit na representação binária
        binario |= (bit << i);
    }

    return binario;
}

int main(int argc, char* argv[])
{
    Img data, secreta;
    Img *image = &data;
    Img *secret = &secreta;
    int i, j, r, t;

    if(argc==3){ // para ocultação
        r = read_ppm(argv[1], image);
        t = read_ppm(argv[2], secret);
        
        //CHECA SE A IMAGEM A SER ESCONDIDA CABE DENTRO DA IMAGEM MAIOR OU NÃO
        if (r == 0)
        {
            printf("width: %d, height: %d\n", image->width, image->height);
            printf("width: %d, height: %d\n", secret->width, secret->height);
            // capacidade de ocultacao na imagem original
            int original = image->width*image->height * 3 * 2;
            // validar se é possível ocultar a image
            // Width*Height da imagem a ser ocultada * 3 * 8 + dimensao da imagem (32 bit para witdh + 32bit para heigth)
            int secreta = secret->width*secret->height * 3 * 8 + 32 + 32;

            if (secreta > original)
            {
                printf("Secret image is too big to hide in this image\n");
                return 0;
            }
        //**************************************************************************    

            

            unsigned char * ptr;
            unsigned char byte;
            unsigned char bitsAOcultar;

            for(int pos=0; pos<(image->width*image->height)*3; pos++)
            {
                unsigned char * ptr = ((unsigned char *) image->pix);
                ptr+=pos;
                byte = *ptr;

                if(pos<16) // ocultando o width da imagem secreta
                { 
                    printf("O valor original do byte é %x => ", byte);
                    printf("O valor de width é %x => ", secret->width);
                    byte = byte & 0b11111100; // 0xFC; 252; ! 0x03 ==> Objetivo é zerar os dois bits menos significativos
                    bitsAOcultar = (secret->width >> ((15-pos)*2)) & 0x03;
                    printf("Esconendo %x em %x", bitsAOcultar, byte);
                    byte = byte | bitsAOcultar;
                    printf(" e o resultado é %x\n",byte);

                    *ptr = byte;
                }
                else if(pos<31) // ocultando o height da imagem secreta
                {
                    byte = byte & 0b11111100; // 0xFC; 252; ! 0x03 ==> Objetivo é zerar os dois bits menos significativos
                    bitsAOcultar = (secret->height >> ((15-(pos-16))*2)) & 0x03;
                    byte = byte | bitsAOcultar;
                    *ptr = byte;
                }
                else // ocultando pixels da imagem secretar
                {

                }
            }

    /*


            //TRANSFORMANDO AS DIMENSÕES DA IMAGEM SECRETA PARA BINÁRIO
            int hbit = secret->height;
            int wbit = secret->width;
            int teste;
            char buffer [33];
            char buffer2 [33];
            itoa(wbit,buffer,2);
            itoa(hbit,buffer2,2);
            printf("binario %s\n", buffer);
            printf("A representação binária de %d é: %s\n", wbit, buffer);
            printf("A representação binária de %d é: %s\n", hbit, buffer2);

            unsigned int mascara = 0xFC;
            itoa(hbit,teste,2);
            teste = teste & mascara;
            printf("A representação binária de %d é: %s\n", hbit, teste);
            
            //**************************************************************************
            //ESCREVENDO A IMAGEM SECRETA NA IMAGEM MAIOR (ja tava aqui)
            // image->pix[50 * image->width + 20].r = 255;
            // image->pix[50 * image->width + 20].g = 255;
            // image->pix[50 * image->width + 20].b = 255;

    */

            write_ppm("baseComImagemOculta.ppm", image);
            free_ppm(image);
            free_ppm(secret);
        }
    }
    else if(argc==2){ // extração
        r = read_ppm(argv[1], image);
        
        //CHECA SE A IMAGEM A SER ESCONDIDA CABE DENTRO DA IMAGEM MAIOR OU NÃO
        if (r == 0)
        {
            printf("width: %d, height: %d\n", image->width, image->height);
            unsigned char * ptr;
            unsigned char byte;
            unsigned char bitsAOcultar;

            int witdhHidden, heightHidden;

            for(int pos=0; pos<32; pos++)
            {
                unsigned char * ptr = ((unsigned char *) image->pix);
                ptr+=pos;
                byte = *ptr;
                if(pos<16) // captura o witdh
                { 
                    unsigned int valor = (unsigned int)(byte & 0x03);
                    valor = valor << (15-pos)*2;
                    witdhHidden = witdhHidden | valor;

                }
                else if(pos<31) // captura o height
                {
                }
                else // 
                {

                }
            }
            printf("O width hidden é %X\n",witdhHidden);
        }
    }

    return 0;
}
