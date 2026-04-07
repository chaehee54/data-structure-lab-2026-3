#include <iostream>

using namespace std;

#define Height 8
#define Width 8

int getMaxBrightness(int img[Height][Width]) {
    int maxVal = img[0][0];

    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            if (img[i][j] > maxVal) {
                maxVal = img[i][j];
            }
        }
    }

    return maxVal;
}

int main() {
    int image[Height][Width] = {
        { 10,  30,  55,  80, 120, 160, 200, 230},
        { 20,  45,  70, 100, 140, 180, 220, 210},
        { 35,  60,  90, 130, 170, 210, 240, 190},
        { 50,  80, 115, 150, 190, 230, 255, 170},
        { 40,  65, 100, 140, 175, 215, 235, 150},
        { 25,  50,  80, 115, 155, 195, 210, 130},
        { 15,  35,  60,  90, 130, 165, 185, 110},
        {  5,  20,  40,  70, 105, 140, 160,  90}
    };

    int result = getMaxBrightness(image);

    cout << result << endl;

    return 0;
}