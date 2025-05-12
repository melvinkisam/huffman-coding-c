// Melvin Erol Sulayman Kisam
// 26002005173
// Imperative Programming
// Huffman Encoder

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *charToBinary (char inputChar)
{
    static char ch[9];
    ch[0] = '\0';

    for (int i = 128; i > 0; i >>= 1)
    {
        strcat(ch, ((inputChar & i) == i) ? "1" : "0");
    }

    return ch;
}

void decode(FILE *fp_out, char table[], char text[], long textSize, int charNum)
{
    int i = 0;
    int size = strlen(text)+1;
    char output[size*8];
    
    while(i < size)
    {
        if(i == 0)
        {
            strcpy(output, charToBinary(text[i]));
        } else
        {
            strcat(output, charToBinary(text[i]));
        }
        i++;
    }
    
    char *inputText;
    long inputFileSize;
    FILE *fp = fopen(table, "r");

    if (fp == NULL)
    {
        printf("Codebook not found!\n");

        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    inputFileSize = ftell(fp);
    rewind(fp);
    inputText = (char *)malloc(inputFileSize + 1 * (sizeof(char)));
    fread(inputText, inputFileSize, 1, fp);
    inputText[inputFileSize] = 0;

    fclose(fp);

    char charArr[100];
    char codesArr[100][100];
    char temp[100];
    memset(temp, 0, 100);
    int charCount = 0;
    int freqCount = 0;
    int tempCount = 0;
    int numOfCodes = 0;

    int j = 0;
    while(j < strlen(inputText))
    {
        if(inputText[j] != '1' && inputText[j] != '0')
        {
            charArr[charCount] = inputText[j];
            charCount++;
            numOfCodes++;           
        } else
        {
            while(1)
            {
                if(inputText[j] == '1')
                {
                    temp[tempCount] = '1';
                    tempCount++;
                } else if(inputText[j] == '0')
                {
                    temp[tempCount] = '0';
                    tempCount++;
                } else
                {
                    int count = 0;
                    while(count < tempCount)
                    {
                        strcpy(codesArr[charCount-1], temp);
                        count++;
                    }
                    tempCount = 0;
                    memset(temp, 0, 100);
                    goto label;
                }
                j++;
            }
        }
        j++;
        label:;
    }

    memset(temp, 0, 100);
    tempCount = 0;
    char decodedText[textSize];
    int decodeCount = 0;

    for(int k = 0; k < strlen(output); k++)
    {
        if(output[k] == '1')
        {
            temp[tempCount] = '1';
            tempCount++;
        } else if(output[k] == '0')
        {
            temp[tempCount] = '0';
            tempCount++;        
        }
        for(int l = 0; l < numOfCodes; l++)
        {
            if(strcmp(temp, codesArr[l]) == 0)
            {
                decodedText[decodeCount] = charArr[l];
                decodeCount++;
                tempCount = 0;
                memset(temp, 0, 100);
                break;
            } 
        }
    }

    char final[charNum];
    for(int k = 0; k < charNum; k++)
    {
        final[k] = decodedText[k];
    }

    printf("Decoded text:\n");
    for(int k = 0; k < charNum; k++)
    {
        printf("%c", final[k]); 
    }
    printf("\n"); 

    for(int k = 0; k < charNum; k++)
    {
        fputc(final[k], fp_out);
    }
}

int main()
{
    char filename[100];
    char codebook[100];
    memset(codebook, 0, 100);
    char metadata[100];
    memset(metadata, 0, 100);
    char *inputText;
    long inputFileSize;
    FILE *fp;
    
    printf("Input a file name to be decoded: ");
    scanf("%[^\n]s", filename);
    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("File not found!\n");

        return 1;
    }

    fseek(fp, 0, SEEK_END);
    inputFileSize = ftell(fp);
    rewind(fp);
    inputText = (char *)malloc(inputFileSize + 1 * (sizeof(char)));
    fread(inputText, inputFileSize, 1, fp);
    inputText[inputFileSize] = 0;

    fclose(fp);

    strcpy(codebook, filename);
    strcat(codebook,".hcodebook");       
    strcpy(metadata, filename);
    strcat(metadata,".metadata");
    char decodedfilename[100] = "decoded_";
    char temp[100];
    memset(temp, 0, 100);
    strcpy(temp, &filename[8]);
    strcat(decodedfilename, temp);

    fp = fopen(metadata, "r");

    if (fp == NULL)
    {
        printf("Metadata not found!\n");

        return 1;
    }

    int numOfChar = getw(fp);
    fclose(fp);

    fp = fopen(decodedfilename, "w+");

    decode(fp, codebook, inputText, inputFileSize, numOfChar);

    fclose(fp);

    printf("\"%s\" successfully created!\n", decodedfilename);

    return 0;
}