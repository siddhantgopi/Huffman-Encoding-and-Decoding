#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
    char data;
    int freq;
    struct Node *left;
    struct Node *right;
};

struct MinHeap
{
    int size;
    int capacity;
    struct Node **array;
};

struct Node *newNode(char data, int freq)
{
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->data = data;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void freeNode(struct Node *node)
{
    free(node);
}

void freeTree(struct Node *root)
{
    if (root == NULL)
    {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    freeNode(root);
}

void swapNodes(struct Node **a, struct Node **b)
{
    struct Node *temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(struct MinHeap *minHeap, int index)
{
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
    {
        smallest = left;
    }
    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
    {
        smallest = right;
    }
    if (smallest != index)
    {
        swapNodes(&minHeap->array[index], &minHeap->array[smallest]);
        minHeapify(minHeap, smallest);
    }
}

int isLeaf(struct Node *node)
{
    return node->left == NULL && node->right == NULL;
}

struct MinHeap *createMinHeap(int capacity)
{
    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct Node **)malloc(capacity * sizeof(struct Node *));
    return minHeap;
}

void insertMinHeap(struct MinHeap *minHeap, struct Node *node)
{
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i > 0 && node->freq < minHeap->array[(i - 1) / 2]->freq)
    {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

struct Node *extractMin(struct MinHeap *minHeap)
{
    struct Node *node = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return node;
}

struct Node *buildHuffmanTree(char *filename)
{
    // Count the frequency of each character in the input file
    int freq[256] = {0};
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error: could not open input file\n");
        exit(1);
    }
    char c;
    while ((c = fgetc(file)) != EOF)
    {
        ++freq[c];
    }
    fclose(file);

    // Build the Huffman tree
    struct MinHeap *minHeap = createMinHeap(256);
    for (int i = 0; i < 256; ++i)
    {
        if (freq[i] > 0)
        {
            insertMinHeap(minHeap, newNode(i, freq[i]));
        }
    }
    while (minHeap->size > 1)
    {
        struct Node *left = extractMin(minHeap);
        struct Node *right = extractMin(minHeap);
        struct Node *parent = newNode('$', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        insertMinHeap(minHeap, parent);
    }
    struct Node *root = extractMin(minHeap);
    free(minHeap->array);
    free(minHeap);
    return root;
}

char *getCodeHelper(struct Node *root, char *code, char data, int index)
{
    if (root == NULL)
    {
        return NULL;
    }
    if (isLeaf(root) && root->data == data)
    {
        code[index] = '\0';
        return code;
    }
    code[index] = '0';
    char *leftCode = getCodeHelper(root->left, code, data, index + 1);
    if (leftCode != NULL)
    {
        return leftCode;
    }
    code[index] = '1';
    char *rightCode = getCodeHelper(root->right, code, data, index + 1);
    if (rightCode != NULL)
    {
        return rightCode;
    }
    return NULL;
}

char *getCode(struct Node *root, char data)
{
    char *code = (char *)malloc(256 * sizeof(char));
    getCodeHelper(root, code, data, 0);
    return code;
}

void writeHuffmanTree(struct Node *root, FILE *out)
{
    if (root == NULL)
    {
        return;
    }
    if (isLeaf(root))
    {
        fputc(root->data, out);
    }
    else
    {
        fputc('#', out);
        writeHuffmanTree(root->left, out);
        writeHuffmanTree(root->right, out);
    }
}

struct Node *readHuffmanTree(FILE *in)
{
    char c = fgetc(in);
    if (c == '#')
    {
        struct Node *node = newNode('$', 0);
        node->left = readHuffmanTree(in);
        node->right = readHuffmanTree(in);
        return node;
    }
    else
    {
        return newNode(c, 0);
    }
}

void encodeTextAndWriteToFile(char *filename, struct Node *root, FILE *out)
{
    FILE *in = fopen(filename, "r");
    if (in == NULL)
    {
        printf("Error: could not open input file\n");
        return;
    }
    char c;
    while ((c = fgetc(in)) != EOF)
    {
        char *code = getCode(root, c);
        fputs(code, out);
    }
    fclose(in);
}

void decodeFileAndWriteText(FILE *in, struct Node *root, FILE *out)
{
    struct Node *current = root;
    char c;
    while ((c = fgetc(in)) != EOF)
    {
        if (c == '0')
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
        if (isLeaf(current))
        {
            fputc(current->data, out);
            current = root;
        }
    }
}

char inputFilename[256];
char outputFilename[256];
struct Node *huffmanTree = NULL;

// Function to build Huffman tree from the input file

// Function to encode the input file and save it to the output file
void encodeFileAndSave()
{
    // Open the input file for encoding
    FILE *inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL)
    {
        MessageBoxA(NULL, "Error: Could not open the input file", "Error", MB_ICONERROR);
        return;
    }

    // Open the output file for saving the encoded data
    FILE *outputFile = fopen(outputFilename, "wb");
    if (outputFile == NULL)
    {
        MessageBoxA(NULL, "Error: Could not open the output file", "Error", MB_ICONERROR);
        fclose(inputFile);
        return;
    }

    // Encode the input text and write it to the binary file
    encodeTextAndWriteToFile(inputFilename, huffmanTree, outputFile);

    // Close files
    fclose(inputFile);
    fclose(outputFile);

    MessageBoxA(NULL, "File encoded and saved successfully!", "Success", MB_ICONINFORMATION);
}

void decodeFileAndSave()
{
    // Open the input file for decoding
    FILE *inputFile = fopen(inputFilename, "rb");
    if (inputFile == NULL)
    {
        MessageBoxA(NULL, "Error: Could not open the input file", "Error", MB_ICONERROR);
        return;
    }

    // Open the output file for saving the decoded data
    FILE *outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL)
    {
        MessageBoxA(NULL, "Error: Could not open the output file", "Error", MB_ICONERROR);
        fclose(inputFile);
        return;
    }

    // Read the Huffman tree from "tree.txt"
    FILE *treeFile = fopen("tree.txt", "r");
    struct Node *root = readHuffmanTree(treeFile);
    fclose(treeFile);

    // Decode the input binary file and write it to the output text file
    decodeFileAndWriteText(inputFile, root, outputFile);

    // Close files
    fclose(inputFile);
    fclose(outputFile);

    // Free the memory used by the Huffman tree
    freeTree(root);

    MessageBoxA(NULL, "File decoded and saved successfully!", "Success", MB_ICONINFORMATION);
}

// Define your window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    OPENFILENAME ofn;    // Declare the file open dialog structure
    char szFile[260];    // Declare the file path buffer
    OPENFILENAME ofnOut; // Declare the file save dialog structure
    char szFileOut[260]; // Declare the file path buffer for saving
    OPENFILENAME ofnOutDecoded;
    char szFileOutDecoded[260];

    char decodedOutputFilename[260];

    HWND hStaticTitle;

    switch (message)
    {
    case WM_CREATE:
        CreateWindowA("STATIC", "Huffman Encoding and Decoding", WS_VISIBLE | WS_CHILD | SS_CENTER , 400, 40, 550, 40, hWnd, NULL, NULL, NULL);
        
        // Create GUI elements (buttons, text boxes, etc.) here
        CreateWindowA("BUTTON", "Select Input File", WS_VISIBLE | WS_CHILD, 425, 120, 500, 50, hWnd, (HMENU)1, NULL, NULL);
        CreateWindowA("BUTTON", "Select Output File", WS_VISIBLE | WS_CHILD, 425, 190, 500, 50, hWnd, (HMENU)2, NULL, NULL);
        CreateWindowA("BUTTON", "Build Huffman Tree", WS_VISIBLE | WS_CHILD, 425, 260, 500, 50, hWnd, (HMENU)3, NULL, NULL);
        CreateWindowA("BUTTON", "Encode and Save", WS_VISIBLE | WS_CHILD, 425, 330, 500, 50, hWnd, (HMENU)4, NULL, NULL);
        CreateWindowA("BUTTON", "Select Output File for Decoded text", WS_VISIBLE | WS_CHILD, 425, 400, 500, 50, hWnd, (HMENU)5, NULL, NULL);
        CreateWindowA("BUTTON", "Decode and Save", WS_VISIBLE | WS_CHILD, 425, 470, 500, 50, hWnd, (HMENU)6, NULL, NULL);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1:
            // Handle input file selection
            ZeroMemory(&ofn, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFile = szFile;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
            ofn.nFilterIndex = 1;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
            if (GetOpenFileName(&ofn))
            {
                // Store the selected file path in inputFilename
                strcpy(inputFilename, ofn.lpstrFile);
            }
            break;
        case 2:
            // Handle output file selection
            ZeroMemory(&ofnOut, sizeof(OPENFILENAME));
            ofnOut.lStructSize = sizeof(OPENFILENAME);
            ofnOut.hwndOwner = hWnd;
            ofnOut.lpstrFile = szFileOut;
            ofnOut.lpstrFile[0] = '\0';
            ofnOut.nMaxFile = sizeof(szFileOut);
            ofnOut.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
            ofnOut.nFilterIndex = 1;
            ofnOut.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
            if (GetSaveFileName(&ofnOut))
            {
                // Store the selected file path in outputFilename
                strcpy(outputFilename, ofnOut.lpstrFile);
            }
            break;
        case 3:
            // Handle building Huffman tree
            huffmanTree = buildHuffmanTree(inputFilename);
            // Write the Huffman tree to a file
            FILE *treeFile = fopen("tree.txt", "w");
            writeHuffmanTree(huffmanTree, treeFile);
            fclose(treeFile);

            MessageBoxA(NULL, "Huffman tree built successfully!", "Success", MB_ICONINFORMATION);
            break;
        case 4:
            // Handle encoding and saving
            encodeFileAndSave();
            break;
        case 5:
            // Handle selecting the output file for decoded text

            ZeroMemory(&ofnOutDecoded, sizeof(OPENFILENAME));
            ofnOutDecoded.lStructSize = sizeof(OPENFILENAME);
            ofnOutDecoded.hwndOwner = hWnd;
            ofnOutDecoded.lpstrFile = szFileOutDecoded;
            ofnOutDecoded.lpstrFile[0] = '\0';
            ofnOutDecoded.nMaxFile = sizeof(szFileOutDecoded);
            ofnOutDecoded.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
            ofnOutDecoded.nFilterIndex = 1;
            ofnOutDecoded.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
            if (GetSaveFileName(&ofnOutDecoded))
            {
                // Store the selected file path in decodedOutputFilename
                strcpy(outputFilename, ofnOutDecoded.lpstrFile);
            }
            break;
        case 6:
            // Handle decoding and saving
            decodeFileAndSave();
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Define the message variable for the message loop
    MSG msg;

    // Define the window class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "MyWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);


    // Register the window class
    if (!RegisterClassEx(&wcex))
    {
        MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create the main window
    HWND hWnd = CreateWindow("MyWindowClass", "Your Application Name", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        MessageBoxA(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Show and update the window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Start the message loop
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}