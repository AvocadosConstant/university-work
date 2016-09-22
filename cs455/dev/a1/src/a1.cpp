#include "interface.hpp"
#include <iostream>

int image_total_pixels(cv::Mat *image) {
    //std::cout << "Image has " << image->rows << " rows and " << image->step << " columns.\n";
    return image->rows * image->step; 
}

std::vector<int> image_histogram(cv::Mat *image) {
    std::vector<int> hist(256, 0);
    int total_pixels = image_total_pixels(image);
    for(int i = 0; i < total_pixels; i++) {
        hist[image->data[i]]++;
    }
    return hist;
}

void image_generate_negative(cv::Mat* image) {
    int total_pixels = image_total_pixels(image);
    for(int i = 0; i < total_pixels; i++) {
        image->data[i] = 255 - image->data[i];
    }
}

void image_histogram_equalize(cv::Mat* image) {
    int total_pixels = image_total_pixels(image);
    std::vector<int> hist = image_histogram(image);
    std::vector<int> cdf(256, 0);

    int sum = 0;
    for(int i = 0; i < hist.size(); i++) {
        sum += hist[i];
        cdf[i] = (sum * 255) / total_pixels;
    }
    for(int i = 0; i < total_pixels; i++) {
        image->data[i] = cdf[image->data[i]];
    }
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
    printf("Calculated threshold: %i\n", threshold);

    // Draw binary image
    for(int i = 0; i < total_pixels; i++) {
        image->data[i] = image->data[i] < threshold ? 0 : 255;
    }
}

std::vector<std::vector<std::pair<int, int> > > image_detect_regions(cv::Mat* image) {
    // Verify binary image
    std::vector<std::vector<std::pair<int, int> > > regions;

    int total_pixels = image_total_pixels(image);
    std::cout << "Image to detect regions on is " << image->step << "x" << image->rows << std::endl;

    std::vector<std::vector<int> > visited;
    visited.resize(image->step, std::vector<int>(image->rows, 0));
    
    for(int y = 0; y < image->rows; y++) {
        for(int x = 0; x < image->step; x++) {
            if((int)image->at<uchar>(x, y) == 255 && visited[x][y] == 0) {
                //std::cout << "Found a new region at (" << x << ", " << y << ")\n";

                std::vector<std::pair<int, int> > region;
                flood_fill(x, y, image, &visited, &region);
                regions.push_back(region);
            }
        }
    }
    std::cout << "Found " << regions.size() << " regions!\n";
    return regions;
}

void flood_fill(int x, int y, cv::Mat* image, std::vector<std::vector<int> > *visited, std::vector<std::pair<int, int> > *region) {
    if( 
        (*visited)[x][y] == 1 
        || (int)image->at<uchar>(x,y) != 255 
        || x < 0 
        || y < 0 
        || x >= image->step 
        || y >= image->rows
    ) {
        //std::cout << "Exiting flood_fill!\n";
        return;
    }

    // Mark as visited
    (*visited)[x][y] = 1;

    // Color with 120
    image->at<uchar>(x,y) = 120;

    // Record point
    std::pair<int, int> point(x, y);
    region->push_back(point);

    flood_fill(x+1, y, image, visited, region);
    flood_fill(x-1, y, image, visited, region);
    flood_fill(x, y+1, image, visited, region);
    flood_fill(x, y-1, image, visited, region);
}
