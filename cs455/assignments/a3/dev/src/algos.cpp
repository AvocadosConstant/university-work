#define PI 3.14159264

#include "driver.hpp"
#include <queue>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <math.h>

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

            pixel[0] = i;//h;
            pixel[1] = i;//s;
            pixel[2] = i;
            
            image->at<cv::Vec3b>(y, x) = pixel;
        }
    }
}

std::vector<std::vector<float> > image_dct(cv::Mat *image, int startX, int startY, int chan) {
    //image->convertTo(*image, CV_32FC3, 1./255);

    // Initialize 8x8 coefficient matrix
    std::vector<std::vector<float> > dct;
    dct.resize(8, std::vector<float>(8, 0));

    // Loop through each coefficient to calculate
    for(int u = 0; u < 8; ++u) {
        for(int v = 0; v < 8; ++v) {

            for(int x = 0; x < 8; x++) {
                for(int y = 0; y < 8; y++) {

                    // Calculate intensity, if out of bounds, set to 0
                    float intensity = (startY+y < image->rows && startX+x < image->cols) ?
                        image->at<cv::Vec3f>(startY + y, startX + x)[chan]
                        : 0;

                    //std::cout << intensity << std::endl;

                    float cosx = cos( (2 * x + 1) * u * PI / (16) );
                    float cosy = cos( (2 * y + 1) * v * PI / (16) );

                    dct[u][v] += intensity * cosx * cosy;
                } 
            }
            float au = (u == 0) ? sqrt(1.0 / 8.0) : sqrt(2.0 / 8.0); 
            float av = (v == 0) ? sqrt(1.0 / 8.0) : sqrt(2.0 / 8.0); 

            dct[u][v] *= au * av;
        }
    }

    /*
    for(int j = 0; j < 8; j++) {
        for(int i = 0; i < 8; i++) {
            std::cout << dct[i][j] << "\t\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    */

    return dct;
}

void image_dct_helper(cv::Mat *image) {
    std::cout << "Applying Discrete Cosine Transform..." << std::endl;
    //cv::Mat dct = image->clone();

    image->convertTo(*image, CV_32FC3);

    // For each channel
    for(int c = 0; c < 3; c++) {
        // Loop through each 8x8 block of image
        for(int x = 0; x < image->cols; x+=8) {
            for(int y = 0; y < image->rows; y+=8) {
                std::vector<std::vector<float> > block = image_dct(image, x, y, c);

                for(int j = 0; j < 8; j++) {
                    for(int i = 0; i < 8; i++) {
                        if(y+j < image->rows && x+i < image->cols) {
                            image->at<cv::Vec3f>(y+j, x+i)[c] = block[i][j];
                        }
                    }
                }
            }
        }
    }
}

// Inverse
std::vector<std::vector<float> > image_idct(cv::Mat *image, int startX, int startY, int chan) {
    // Initialize 8x8 pixel matrix
    std::vector<std::vector<float> > block;
    block.resize(8, std::vector<float>(8, 0));

    // Loop through each pixel
    for(int x = 0; x < 8; ++x) {
        for(int y = 0; y < 8; ++y) {

            for(int u = 0; u < 8; u++) {
                for(int v = 0; v < 8; v++) {

                    float au = (u == 0) ? sqrt(1.0 / 8.0) : sqrt(2.0 / 8.0);
                    float av = (v == 0) ? sqrt(1.0 / 8.0) : sqrt(2.0 / 8.0);

                    float coeff = (startY+v < image->rows && startX+u < image->cols) ?
                        image->at<cv::Vec3b>(startY + v, startX + u)[chan]
                        : 0;

                    //std::cout << coeff << std::endl;

                    float cosx = cos( (2 * x + 1) * u * PI / (16) );
                    float cosy = cos( (2 * y + 1) * v * PI / (16) );

                    block[x][y] += au * av * coeff* cosx * cosy;
                } 
            }
        }
    }
    for(int j = 0; j < 8; j++) {
        for(int i = 0; i < 8; i++) {
            std::cout << block[i][j] << "\t\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    return block;
}

void image_idct_helper(cv::Mat *image) {
    std::cout << "Applying Inverse Discrete Cosine Transform..." << std::endl;
    //cv::Mat idct = image->clone();

    image->convertTo(*image, CV_8UC3);

    // For each channel
    for(int c = 0; c < 3; c++) {
        // Loop through each 8x8 block of image
        for(int x = 0; x < image->cols; x+=8) {
            for(int y = 0; y < image->rows; y+=8) {

                // Calculate 8x8 pixel block from coefficients
                std::vector<std::vector<float> > block = image_idct(image, x, y, c);

                for(int j = 0; j < 8; j++) {
                    for(int i = 0; i < 8; i++) {
                        if(y+j < image->rows && x+i < image->cols) {
                            image->at<cv::Vec3b>(y+j, x+i)[c] = block[i][j];
                        }
                    }
                }
            }
        }
    }
}
