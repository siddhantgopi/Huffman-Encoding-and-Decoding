#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char data;
    int freq;
    struct Node* left;
    struct Node* right;
};

struct MinHeap {
    int size;
    int capacity;
    struct Node** array;
};

struct Node* newNode(char data, int freq) {
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    node->data = data;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void freeNode(struct Node* node) {
    free(node);
}

void freeTree(struct Node* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    freeNode(root);
}

void swapNodes(struct Node** a, struct Node** b) {
    struct Node* temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(struct MinHeap* minHeap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq) {
        smallest = left;
    }
    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq) {
        smallest = right;
    }
    if (smallest != index) {
        swapNodes(&minHeap->array[index], &minHeap->array[smallest]);
        minHeapify(minHeap, smallest);
    }
}

int isLeaf(struct Node* node) {
    return node->left == NULL && node->right == NULL;
}

struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap* minHeap = (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct Node**) malloc(capacity * sizeof(struct Node*));
    return minHeap;
}

void insertMinHeap(struct MinHeap* minHeap, struct Node* node) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i > 0 && node->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

struct Node* extractMin(struct MinHeap* minHeap) {
    struct Node* node = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return node;
}



struct Node* buildHuffmanTree(char* filename) {
    // Count the frequency of each character in the input file
    int freq[256] = {0};
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: could not open input file\n");
        exit(1);
    }
    char c;
    while ((c = fgetc(file)) != EOF) {
        ++freq[c];
    }
    fclose(file);

    // Build the Huffman tree
    struct MinHeap* minHeap = createMinHeap(256);
    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            insertMinHeap(minHeap, newNode(i, freq[i]));
        }
    }
    while (minHeap->size > 1) {
        struct Node* left = extractMin(minHeap);
        struct Node* right = extractMin(minHeap);
        struct Node* parent = newNode('$', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        insertMinHeap(minHeap, parent);
    }
    struct Node* root = extractMin(minHeap);
    free(minHeap->array);
    free(minHeap);
    return root;
}

char* getCodeHelper(struct Node* root, char* code, char data, int index) {
    if (root == NULL) {
        return NULL;
    }
    if (isLeaf(root) && root->data == data) {
        code[index] = '\0';
        return code;
    }
    code[index] = '0';
    char* leftCode = getCodeHelper(root->left, code, data, index + 1);
    if (leftCode != NULL) {
        return leftCode;
    }
    code[index] = '1';
    char* rightCode = getCodeHelper(root->right, code, data, index + 1);
    if (rightCode != NULL) {
        return rightCode;
    }
    return NULL;
}

char* getCode(struct Node* root, char data) {
    char* code = (char*) malloc(256 * sizeof(char));
    getCodeHelper(root, code, data, 0);
    return code;
}

void writeHuffmanTree(struct Node* root, FILE* out) {
    if (root == NULL) {
        return;
    }
    if (isLeaf(root)) {
        fputc(root->data, out);
    } else {
        fputc('#', out);
        writeHuffmanTree(root->left, out);
        writeHuffmanTree(root->right, out);
    }
}

struct Node* readHuffmanTree(FILE* in) {
    char c = fgetc(in);
    if (c == '#') {
        struct Node* node = newNode('$', 0);
        node->left = readHuffmanTree(in);
        node->right = readHuffmanTree(in);
        return node;
    } else {
        return newNode(c, 0);
    }
}

void encodeTextAndWriteToFile(char* filename, struct Node* root, FILE* out) {
    FILE* in = fopen(filename, "r");
    if (in == NULL) {
        printf("Error: could not open input file\n");
        return;
    }
    char c;
    while ((c = fgetc(in)) != EOF) {
        char* code = getCode(root, c);
        fputs(code, out);
    }
    fclose(in);
}

void decodeFileAndWriteText(FILE* in, struct Node* root, FILE* out) {
    struct Node* current = root;
    char c;
    while ((c = fgetc(in)) != EOF) {
        if (c == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        if (isLeaf(current)) {
            fputc(current->data, out);
            current = root;
        }
    }
}

int main() {
    char filename[100];

    int n;
    printf(" Encoding - 1 \n Decoding - 2 \n Enter option : ");
    scanf("%d", &n);

    if(n == 1){

    printf("Enter the name of the input file: ");
    scanf("%s", filename);

    // Build the Huffman tree
    struct Node* root = buildHuffmanTree(filename);

    // Write the Huffman tree to a file
    FILE* treeFile = fopen("tree.txt", "w");
    writeHuffmanTree(root, treeFile);
    fclose(treeFile);

    // Encode the input text and write it to a binary file
    FILE* inputFile = fopen(filename, "r");
    FILE* outputFile = fopen("encoded.bin", "wb");
    encodeTextAndWriteToFile(filename, root, outputFile);
    fclose(inputFile);
    fclose(outputFile);

    freeTree(root);
    }

    else if(n == 2){

    FILE* treeFile = fopen("tree.txt", "r");
    struct Node* root = readHuffmanTree(treeFile);
    fclose(treeFile);

    // Decode the binary file and write it to a text file
    FILE* inputFile = fopen("encoded.bin", "rb");
    FILE* outputFile = fopen("decoded.txt", "w");
    decodeFileAndWriteText(inputFile, root, outputFile);
    fclose(inputFile);
    fclose(outputFile);

    // Free the memory used by the Huffman tree
    freeTree(root);

    }

    return 0;
}