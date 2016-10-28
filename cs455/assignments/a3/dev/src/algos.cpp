#define PI 3.14159264

#include "driver.hpp"
#include <queue>
#include <iostream>
#include <algorithm>
#include <iomanip>

int image_total_pixels(cv::Mat *image) {
    return image->rows * image->step; 
}

void image_rgb_to_hsi(cv::Mat *image) {
    std::cout << "Converting image from RGB to HSI..." << std::endl;
    float b, g, r, bgr, h, s, i;

    for(int x = 0; x < image->cols; x++) {
        for(int y = 0; y < image->rows; y++) {

            // Extract channel values of pixel
            cv::Vec3b &pixel = image->at<cv::Vec3b>(y, x);

            b = pixel[0];
            g = pixel[1];
            r = pixel[2];

            // Normalize values
            bgr = b + g + r;
            b = b / bgr;
            g = g / bgr;
            r = r / bgr;

            // Hue
            h = 0.5 * ((r - g) + (r - b)) / sqrt(((r - g)*(r - g)) + ((r - b)*(g - b)));
            h = 180 * acos(h) / PI;
            if(b > g) h = 360 - h;
            h = 255 * h / 360;

            // Saturation
            float min = std::min(r, std::min(b,g));
            s = (1 - (3 * min)) * 255;

            // Intensity
            i = bgr / 3;

            pixel[0] = h;
            pixel[1] = s;
            pixel[2] = i;
            
            image->at<cv::Vec3b>(y, x) = pixel;
        }
    }
}
