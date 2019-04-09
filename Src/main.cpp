#include <iostream>
#include "image.hpp"
#include "thumbnail.hpp"

int main() {

    apImage image(50, 50);
    apThumbnail th;

    for (int i=0; i<50; i++) {
        for (int j=0; j<50; j++) {
            image.setPixel(i, j, (unsigned char)i+j);
        }
    }

    apImage i2 = image;
    apImage i3(image);

    th.createThumbnail("input.txt", "output.txt", 1);
    
    return 0;
}