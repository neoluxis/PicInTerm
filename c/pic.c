#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BITMAP_FILE_PATH "furina.bmp"  // Bitmap文件的路径
#define CHARACTER "#"  // 在控制台用来显示图片的字符

struct Pixel {
    unsigned char B, G, R;
};

struct Pixel *pixels = NULL;    // 图片的所有像素
int width, height;              // 图片的 宽度 和 高度

#pragma pack(1)
typedef struct {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} BITMAPINFOHEADER;
#pragma pack()

// 读取Bitmap图片
int readBitmap(const char* filePath) {
    FILE *file = fopen(filePath, "rb");
    if (file == NULL)
        return -1;

    // 读取Bitmap的文件头部分
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);
    width = infoHeader.biWidth;
    height = infoHeader.biHeight;

    // 读取Bitmap图片的数据
    pixels = malloc(sizeof(struct Pixel) * width * height);
    fseek(file, fileHeader.bfOffBits, SEEK_SET);
    fread(pixels, sizeof(struct Pixel), width * height, file);

    // Bitmap图片的行顺序是上下颠倒的，这里把它颠倒回来
    struct Pixel* temp = malloc(sizeof(struct Pixel) * width * height);
    for (int y = 0; y < height; y++)
        memcpy(temp + y * width, pixels + (height - y - 1) * width, width * sizeof(struct Pixel));
    free(pixels);
    pixels = temp;

    fclose(file);
    return 1;
}

int main(int argc, char *argv[]) {

    // 读取Bitmap图片数据
    if (readBitmap(BITMAP_FILE_PATH) < 0) {
        printf("文件 '" BITMAP_FILE_PATH "' 打开失败！\n");
        return -1;
    }

    // 清屏
//    printf("\033[2J");
//    printf("\033[H");

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            struct Pixel pixel = pixels[x + y * width];  // 获取图片中 (x, y) 点的像素值

            // printf("\033[5m");  // 闪烁效果
            // printf("\033[1m");  // 高亮度
            // printf("\033[48;2;%d;%d;%dm", pixel.R, pixel.G, pixel.B);  // 设置文本背景色

            printf("\033[38;2;%d;%d;%dm", pixel.R, pixel.G, pixel.B);  // 设置前景色(文本颜色)

            printf(CHARACTER); // 打印字符
        }
        putchar('\n');
    }

    // 清除文字样式
    printf("\033[0m");

    free(pixels);

    return 0;
}

