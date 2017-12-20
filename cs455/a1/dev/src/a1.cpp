#include "interface.hpp"
#include <queue>
#include <iostream>
#include <algorithm>

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
    //printf("Calculated threshold: %i\n", threshold);

    // Draw binary image
    for(int i = 0; i < total_pixels; i++) {
        image->data[i] = image->data[i] < threshold ? 0 : 255;
    }
}

std::vector<std::vector<std::pair<int, int> > > image_detect_regions(cv::Mat* image) {
    std::vector<std::vector<std::pair<int, int> > > regions;

    //int total_pixels = image_total_pixels(image);
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

void image_detect_regions_2(cv::Mat* image) {
    int shade = 0;
    int size = image_total_pixels(image);
    int width = image->size().width;
    std::queue<int> toVisit;
    std::vector<bool> visited(size, false);
    std::vector<std::vector<int> > regions;
    bool foundNewRegion = false;

    for(int i = 0; i < size; i++) {
        std::vector<int> region;
        if(!visited[i] && image->data[i] == 255) {
            toVisit.push(i);
            visited[i] = true;
            shade = (shade + 50) % 255;
            foundNewRegion = true;
        }
        while(!toVisit.empty()) {
            int pix = toVisit.front(); toVisit.pop();
            region.push_back(pix);
            image->data[pix] = 120;
            int left = pix - 1, 
                right = pix + 1, 
                up = pix - width, 
                down = pix + width;

            if(left % width != 0 && !visited[left] && image->data[left] == 255) {
                toVisit.push(left); 
                visited[left] = true;
            }
            if(right % width != 0 && !visited[right] && image->data[right] == 255) {
                toVisit.push(right); 
                visited[right] = true;
            }
            if(up >= 0 && !visited[up] && image->data[up] == 255) {
                toVisit.push(up); 
                visited[up] = true;
            }
            if(down < size && !visited[down] && image->data[down] == 255) {
                toVisit.push(down); 
                visited[down] = true;
            }
        }
        if(foundNewRegion) {
            regions.push_back(region);
            foundNewRegion = false;
        }
    }
    
    //for(int i = 0; i < regions.size(); i++) {
    //    if(regions[i].size() < 50) {
    //        regions.erase(regions.begin() + i);
    //        i--;
    //    }
    //}

    regions.erase(
        std::remove_if(
            regions.begin(), 
            regions.end(), 
            [](const std::vector<int> &a) {
                return a.size() < 50;
            }
        ),
        regions.end()
    );
    
    std::sort(
        regions.begin(), 
        regions.end(), 
        [](const std::vector<int> &a, const std::vector<int> &b) {
            return a.size() < b.size();
        }
    );

    if(regions.size() <= 0) return;

    std::cout << regions.size() << " regions detected!\n";
    
    for(int i = 1; i <= regions.size(); i++) {
        std::cout << "Region " << i << " has " << regions[i-1].size() << " pixels\n";
    }
    
    std::cout << "Smallest region: " << regions.front().size() << " | Largest region: " << regions.back().size() << std::endl;

    for(int i = 0; i < regions.front().size(); i++) {
        image->data[regions.front()[i]] = 60;
    }

    for(int i = 0; i < regions.back().size(); i++) {
        image->data[regions.back()[i]] = 200;
    }
}
