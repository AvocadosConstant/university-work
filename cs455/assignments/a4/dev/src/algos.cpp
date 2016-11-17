#include "driver.hpp"
#include <queue>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <opencv2/imgproc/imgproc.hpp> 

int image_total_pixels(cv::Mat *image) {
    return image->rows * image->step; 
}

void image_generate_negative(cv::Mat* image) {
    int total_pixels = image_total_pixels(image);
    for(int i = 0; i < total_pixels; i++) {
        image->data[i] = 255 - image->data[i];
    }
}

void image_convert_grayscale(cv::Mat *image) {
        cv::cvtColor(*image, *image, CV_BGR2GRAY);
}

std::vector<int> image_histogram(cv::Mat *image) {
    std::vector<int> hist(256, 0);
    int total_pixels = image_total_pixels(image);
    for(int i = 0; i < total_pixels; i++) {
        hist[image->data[i]]++;
    }
    return hist;
}

void image_generate_binary(cv::Mat* image) {
    int total_pixels = image_total_pixels(image);
    std::vector<int> hist = image_histogram(image);
    // Calculate threshold
    int threshold = 0;
    for(int i = 0; i < hist.size(); i++) {
        threshold += i * hist[i];
    }
    threshold /= total_pixels;
    //printf("Calculated threshold: %i\n", threshold);

    // Draw binary image
    for(int i = 0; i < total_pixels; i++) {
        image->data[i] = image->data[i] < threshold ? 0 : 255;
    }
}
