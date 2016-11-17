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


// Morphological Algos

void image_dilate(cv::Mat* image, std::vector<Point> struc) {

    // Loop through each pixel in image
    for(int y = 0; y < image->rows; y++) {
        for(int x = 0; x < image->step; x++) {

            // Loop through each point defined in structure element
            for(auto p : struc) {

                int offsetX = x + p.x;
                int offsetY = y + p.y;

                // Check if offset of point to the current pixel is within bounds
                if(offsetX >= 0 && offsetX < image->step && offsetY >= 0 && offsetY < image->rows){
                    
                    // If offset point is white, make current px gray
                    if(image->at<uchar>(offsetY, offsetX) == 255) {
                        image->at<uchar>(y, x) = 120;
                    }
                }
            }
        }
    }
    for(int y = 0; y < image->rows; y++) {
        for(int x = 0; x < image->step; x++) {
            if(image->at<uchar>(y, x) == 120) image->at<uchar>(y, x) = 255;
        }
    }
}

void image_erode(cv::Mat* image, std::vector<Point> struc) {

    std::vector<Point> erosion;

    // Loop through each pixel in image
    for(int y = 0; y < image->rows; y++) {
        for(int x = 0; x < image->step; x++) {

            bool erode = true;

            // Loop through each point defined in structure element
            for(auto p : struc) {

                int offsetX = x + p.x;
                int offsetY = y + p.y;

                // Check if offset of point to the current pixel is within bounds
                if(offsetX >= 0 && offsetX < image->step && offsetY >= 0 && offsetY < image->rows){
        
                    // If offset point is white, make current px gray
                    if(image->at<uchar>(offsetY, offsetX) == 0) erode = false;
                }
            }
            if(erode) erosion.push_back(Point{x, y});
        }
    }
    for(int y = 0; y < image->rows; y++) {
        for(int x = 0; x < image->step; x++) {
            image->at<uchar>(y, x) = 0;
        }
    }
    for(auto p : erosion) {
        image->at<uchar>(p.y, p.x) = 255;
    }
}
