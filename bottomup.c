#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: copy infile outfile\n");
        return 1;
    }

    char *infile = argv[1];
    char *outfile = argv[2];

    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 3;
    }

    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 4;
    }

    for (int i = bi.biHeight - 1; i >= 0; i--)
    {
        fseek(inptr, bf.bfOffBits + (bi.biWidth * sizeof(RGBTRIPLE) + padding) * i, SEEK_SET);

        for (int j = 0; j < bi.biWidth; j++)
        {
            RGBTRIPLE triple;
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    fclose(inptr);

    fclose(outptr);

    return 0;
}