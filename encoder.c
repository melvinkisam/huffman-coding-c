// Melvin Erol Sulayman Kisam
// 26002005173
// Imperative Programming
// Huffman Encoder

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int bytes = 0;

// function to list unique characters and number of character occurences
void characterFrequencyList(char x[], int y[], char text[], int textSize, int *arrSize)
{
    int count = 0;
    int pos = 0;

    while (count < textSize)
    {
        char c = text[count];

        // assign the first character from the first iteration
        if (count == 0)
        {
            x[0] = text[0];
            y[0] = 1;
            pos++;
        }
        else
        {
            for (int i = 0; i < count; i++)
            {
                // check if the same character exist
                if (c == text[i])
                {
                    for (int j = 0; j < count; j++)
                    {
                        // increase frequency of the character
                        if (x[j] == text[i])
                        {
                            y[j]++;
                            break;
                        }
                    }
                    break;
                }
                // check if the character is unique
                if (i == (count - 1))
                {
                    // store the unique character
                    x[pos] = c;
                    y[pos] = 1;
                    pos++;
                }
            }
        }
        count++;
    }
    *arrSize = pos;
}

// function to swap position
void swapChar(char *x, char *y)
{
    char temp = *x;
    *x = *y;
    *y = temp;
}

void swapInt(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

// bubble sort algorithm
void bubbleSort(char x[], int y[], int arrSize)
{
    for (int i = 0; i < arrSize - 1; i++)
        // last i elements are already in place
        for (int j = 0; j < arrSize - i - 1; j++)
            if (y[j] > y[j + 1])
            {
                // swap values of frequency array
                swapChar(&x[j], &x[j + 1]);
                // swap values of character array
                swapInt(&y[j], &y[j + 1]);
            }
}

struct MinHNode
{
    char character;
    unsigned freq;
    struct MinHNode *left, *right;
};

struct MinHeap
{
    unsigned size;
    unsigned capacity;
    struct MinHNode **array;
};

// function to create nodes
struct MinHNode *newNode(char character, unsigned freq)
{
    struct MinHNode *temp = (struct MinHNode *)malloc(sizeof(struct MinHNode));

    temp->left = temp->right = NULL;
    temp->character = character;
    temp->freq = freq;

    return temp;
}

// function to create min heap
struct MinHeap *createMinH(unsigned capacity)
{
    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));

    minHeap->size = 0;

    minHeap->capacity = capacity;

    minHeap->array = (struct MinHNode **)malloc(minHeap->capacity * sizeof(struct MinHNode *));

    return minHeap;
}

// function to swap nodes
void swapMinHNode(struct MinHNode **a, struct MinHNode **b)
{
    struct MinHNode *t = *a;
    *a = *b;
    *b = t;
}

// function to heapify
void minHeapify(struct MinHeap *minHeap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx)
    {
        swapMinHNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// function to extract min
struct MinHNode *extractMin(struct MinHeap *minHeap)
{
    struct MinHNode *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

// function to perform insertion
void insertMinHeap(struct MinHeap *minHeap, struct MinHNode *minHeapNode)
{
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq)
    {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap *minHeap)
{
    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

struct MinHeap *createAndBuildMinHeap(char character[], int freq[], int size)
{
    struct MinHeap *minHeap = createMinH(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(character[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

struct MinHNode *buildHuffmanTree(char character[], int freq[], int size)
{
    struct MinHNode *left, *right, *top;
    struct MinHeap *minHeap = createAndBuildMinHeap(character, freq, size);

    while (!(minHeap->size == 1))
    {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

// function to write the huffman codes to an output file
void outputHCodes(char c, int arr[], int size, FILE *output)
{
    fprintf(output, "%c", c);
    for (int i = 0; i < size; i++)
    { 
        fprintf(output, "%d", arr[i]);
    }
    //fprintf(output, "\n");
}

// Print the array
void printArray(int arr[], int n)
{
    for (int i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf("\n");
}

void printHCodes(struct MinHNode *root, int arr[], int top, FILE *output)
{
    if (root->left)
    {
        arr[top] = 0;
        printHCodes(root->left, arr, top + 1, output);
    }
    if (root->right)
    {
        arr[top] = 1;
        printHCodes(root->right, arr, top + 1, output);
    }
    if (!(root->left) && !(root->right))
    {
        char c = root->character;
        outputHCodes(c, arr, top, output);
        printf("%c : ", c);
        printArray(arr, top);
    }
}

// Wrapper function
void HuffmanCodes(char character[], int freq[], int size, FILE *output)
{
    int arr[100];
    int top = 0;

    struct MinHNode *root = buildHuffmanTree(character, freq, size);

    printHCodes(root, arr, top, output);
}

// function to encode the text using the huffman codebook
void encode(FILE *fp_out, char table[], char text[])
{
    int ch;
    int i = 0;
    char chunk;
    int bitCount = 0;

    while(i < strlen(text))
    {
        FILE *fp_in = fopen(table, "r");

        while(1)
        {
            ch = fgetc(fp_in);

            if(ch == text[i])
            {
                ch = fgetc(fp_in);
                while(ch == '1' || ch == '0')
                {   
                    if(bitCount == 7)
                    {
                        if(ch == '1')
                        {
                            chunk |= (1 << (7-bitCount));
                        } else if(ch == '0')
                        {
                            chunk |= (0 << (7-bitCount));
                        }
                        fputc(chunk, fp_out);
                        chunk = 0;
                        bitCount = 0;
                        bytes++;
                        goto label;
                    }                 
                    if(ch == '1')
                    {
                        chunk |= (1 << (7-bitCount)); 
                    } else if(ch == '0')
                    {
                        chunk |= (0 << (7-bitCount));                        
                    }
                    bitCount++;
                    label:;             
                    ch = fgetc(fp_in);                
                }
                break;
            }          
        }
        fclose(fp_in);
        i++;
    }
    if(bitCount != 0)
    {
        while(1)
        {
            if(bitCount == 7)
                break;
            chunk |= (0 << (7-bitCount)); 
            bitCount++;      
        }
        bytes++;
        fputc(chunk, fp_out);           
    }   
    chunk = 0;
    bitCount = 0;  
}

int main()
{
    int input;
    int sizeOfArr;
    int newArrSize;

    printf("Input options: (pick number 1 or 2)\n");
    printf("1. Input file\n");
    printf("2. Console\n");
    scanf("%d", &input);

    if (input == 1)
    {
        // text file input
        char filename[100];
        char codebook[100];
        memset(codebook, 0, 100);
        char metadata[100];
        memset(metadata, 0, 100);
        char *inputText;
        long inputFileSize;
        FILE *fp;

        while ((getchar()) != '\n'); // removes the '\n' char from scanf
        printf("Input a file name to be encoded: ");
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
        sizeOfArr = strlen(inputText);

        fclose(fp);

        char encodedfilename[100] = "encoded_";
        strcat(encodedfilename, filename);
        strcpy(codebook, encodedfilename);
        strcat(codebook,".hcodebook");       
        strcpy(metadata, encodedfilename);
        strcat(metadata,".metadata");

        char charArr[sizeOfArr];
        int freqArr[sizeOfArr];

        characterFrequencyList(charArr, freqArr, inputText, sizeOfArr, &newArrSize);
        bubbleSort(charArr, freqArr, newArrSize);

        printf("================\n");
        printf("Huffman codebook\n");
        printf("================\n");
        fp = fopen(codebook, "w+");
        HuffmanCodes(charArr, freqArr, newArrSize, fp);
        printf("================\n");
        printf("\"%s\" successfully created!\n", codebook);
        fclose(fp);

        fp = fopen(encodedfilename, "w+");
        encode(fp, codebook, inputText);
        printf("\"%s\" successfully created!\n", encodedfilename);
        fclose(fp);

        fp = fopen(metadata, "w+");
        putw(sizeOfArr, fp);
        printf("\"%s\" successfully created!\n", metadata);
        fclose(fp);

        printf("Total characters: %d\n", sizeOfArr);
        printf("Total number of bytes: %d\n", bytes);
        double bits = (bytes*8.0)/sizeOfArr;
        printf("Average bits per symbol: %f\n", bits);
    }
    else if (input == 2)
    {
        // console input
        char inputConsole[100];
        FILE *fp;

        while ((getchar()) != '\n'); // removes the '\n' char from scanf
        printf("Enter a text to be encoded: ");
        scanf("%[^\n]s", inputConsole);

        sizeOfArr = strlen(inputConsole);
        char charArr[sizeOfArr];
        int freqArr[sizeOfArr]; 

        characterFrequencyList(charArr, freqArr, inputConsole, sizeOfArr, &newArrSize);
        bubbleSort(charArr, freqArr, newArrSize);

        printf("================\n");
        printf("Huffman codebook\n");
        printf("================\n");
        fp = fopen("encoded_text.txt.hcodebook", "w+");
        HuffmanCodes(charArr, freqArr, newArrSize, fp);
        printf("================\n");
        printf("\"encoded_text.txt.hcodebook\" successfully created!\n");
        fclose(fp);

        fp = fopen("encoded_text.txt", "w+");
        encode(fp, "encoded_text.txt.hcodebook", inputConsole);
        printf("\"encoded_text.txt\" successfully created!\n");
        fclose(fp);

        fp = fopen("encoded_text.txt.metadata", "w+");
        putw(sizeOfArr, fp);
        printf("\"encoded_text.txt.metadata\" successfully created!\n");
        fclose(fp);

        printf("Total characters: %d\n", sizeOfArr);
        printf("Total number of bytes: %d\n", bytes);
        double bits = (bytes*8.0)/sizeOfArr;
        printf("Average bits per symbol: %f\n", bits);
    }
    else
    {
        printf("Invalid input!\n");

        return 1;
    }

    return 0;
}