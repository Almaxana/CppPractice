#include <iostream>
#include <deque>

const unsigned char BYTES_PER_PIXEL = 3; /// red, green, blue
const unsigned char FILE_HEADER_SIZE = 14;
const unsigned char INFO_HEADER_SIZE = 40;
const unsigned char BYTES_PER_ONE_COLOR = 1;

void generateBitmapImage(unsigned char*** image, uint16_t length, uint16_t width, char* imageFileName);
unsigned char* createBitmapFileHeader(uint16_t length, uint64_t fullLine);
unsigned char* createBitmapInfoHeader(uint16_t length, uint16_t width);


void bmp (uint16_t length, uint16_t width, std::deque<std::deque<int64_t>>& matrix, std::string outpt, uint64_t numberImage) {

    unsigned char*** image = new unsigned char** [length];///table for color storage
    for(size_t i = 0; i < length; i++){
        image[i] = new unsigned char* [width];
        for(size_t j = 0; j < width; j++){
            image[i][j] = new unsigned char [BYTES_PER_PIXEL];
        }
    }
    outpt = outpt + "\\" + std::to_string(numberImage) + ".bmp";
    char* imageFileName = (char*) outpt.c_str();

    for (size_t i = 0; i < length; i++) {
        for (size_t j = 0; j < width; j++) {

            if(matrix[i][j]==0){
                image[i][j][2] = (unsigned char) ( 255 );///red
                image[i][j][1] = (unsigned char) ( 255 );///green
                image[i][j][0] = (unsigned char) ( 255);///blue
            }
            if(matrix[i][j]==1){
                image[i][j][2] = (unsigned char) ( 0 );///red
                image[i][j][1] = (unsigned char) ( 255 );///green
                image[i][j][0] = (unsigned char) ( 0);///blue
            }
            if(matrix[i][j]==2){
                image[i][j][2] = (unsigned char) ( 255 );///red
                image[i][j][1] = (unsigned char) ( 0 );///green
                image[i][j][0] = (unsigned char) ( 255);///blue
            }
            if(matrix[i][j]==3){
                image[i][j][2] = (unsigned char) ( 255 );///red
                image[i][j][1] = (unsigned char) ( 255 );///green
                image[i][j][0] = (unsigned char) ( 0);///blue
            }
            if(matrix[i][j]>3){
                image[i][j][2] = (unsigned char) ( 0 );///red
                image[i][j][1] = (unsigned char) ( 0 );///green
                image[i][j][0] = (unsigned char) ( 0);///blue
            }

        }
    }

    generateBitmapImage(image, length, width, imageFileName);
}

void generateBitmapImage (unsigned char*** image, uint16_t length, uint16_t width, char* imageFileName) {
    uint64_t widthInBytes = width * BYTES_PER_PIXEL;

    unsigned char padding[3] = {0, 0, 0};
    unsigned char paddingSize = (4 - (widthInBytes) % 4) % 4;

    uint64_t fullLine = (widthInBytes) + paddingSize;///size of a line with padding

    FILE* imageFile = fopen(imageFileName, "wb");

    unsigned char* fileHeader = createBitmapFileHeader(length, fullLine);
    fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

    unsigned char* infoHeader = createBitmapInfoHeader(length, width);
    fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);

    for (int32_t i = length - 1; i >= 0; i--) { ///writing from down left
        for(size_t j = 0; j < width; j++){
            fwrite(image[i][j], BYTES_PER_ONE_COLOR, BYTES_PER_PIXEL, imageFile);
        }
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char* createBitmapFileHeader (uint16_t length, uint64_t fullLine) {


    static unsigned char fileHeader[] = {
            0,0,     /// signature
            0,0,0,0, /// image file size in bytes
            0,0,0,0, /// reserved
            0,0,0,0, /// start of pixel array in bytes
    };

    ///FILE_HEADER_SIZE + INFO_HEADER_SIZE + (fullLine * length) = fileSize;
    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE + (fullLine * length)     );
    fileHeader[ 3] = (unsigned char)((FILE_HEADER_SIZE + INFO_HEADER_SIZE + (fullLine * length)) >>  8);
    fileHeader[ 4] = (unsigned char)((FILE_HEADER_SIZE + INFO_HEADER_SIZE + (fullLine * length)) >> 16);
    fileHeader[ 5] = (unsigned char)((FILE_HEADER_SIZE + INFO_HEADER_SIZE + (fullLine * length)) >> 24);
    fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

    return fileHeader;
}

unsigned char* createBitmapInfoHeader (uint16_t length, uint16_t width) {
    static unsigned char infoHeader[] = {
            0,0,0,0, /// header size
            0,0,0,0, /// image width
            0,0,0,0, /// image length
            0,0,     /// number of color planes
            0,0,     /// bits per pixel = 24
            0,0,0,0, /// compression
            0,0,0,0, /// image size
            0,0,0,0, /// horizontal resolution
            0,0,0,0, /// vertical resolution
            0,0,0,0, /// colors in color table
            0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(INFO_HEADER_SIZE);
    infoHeader[ 4] = (unsigned char)(width      );
    infoHeader[ 5] = (unsigned char)(width >>  8);
    infoHeader[ 6] = (unsigned char)(width >> 16);
    infoHeader[ 7] = (unsigned char)(width >> 24);
    infoHeader[ 8] = (unsigned char)(length      );
    infoHeader[ 9] = (unsigned char)(length >> 8);
    infoHeader[10] = (unsigned char)(length >> 16);
    infoHeader[11] = (unsigned char)(length >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(BYTES_PER_PIXEL*8);

    return infoHeader;
}