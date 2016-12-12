#include "driver.hpp"
#include <queue>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <opencv2/imgproc/imgproc.hpp> 


void image_convert_grayscale(cv::Mat *image) {
    std::cout << "Converting image to grayscale... " << std::endl;
    cv::cvtColor(*image, *image, CV_BGR2GRAY);
}

void image_convert_color(cv::Mat *image) {
    std::cout << "Converting image to RGB... " << std::endl;
    cv::cvtColor(*image, *image, CV_GRAY2RGB);
}

int image_total_pixels(cv::Mat *image) {
    return image->rows * image->step; 
}

void image_generate_negative(cv::Mat* image) {
    int total_pixels = image_total_pixels(image);
    for(int i = 0; i < total_pixels; i++) {
        image->data[i] = 255 - image->data[i];
    }
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
    threshold = 200;

    // Draw binary image
    for(int i = 0; i < total_pixels; i++) {
        image->data[i] = image->data[i] < threshold ? 0 : 255;
    }
}

int get_virtual_px_val(cv::Mat* image, int x, int y) {
    if(x < 0) {
        return get_virtual_px_val(image, 0, y);
    } else if(x >= image->step) {
        return get_virtual_px_val(image, image->step - 1, y);
    } else if(y < 0) {
        return get_virtual_px_val(image, x, 0);
    } else if(y >= image->rows) {
        return get_virtual_px_val(image, x, image->rows - 1);
    }
    return image->at<uchar>(y, x);
}

void image_apply_kernel(cv::Mat* image, std::vector<int>* kernel) {
    std::cout << "Applying kernel..." << std::endl;

    int kernel_weight = 0;
    int kernel_dim = std::sqrt(kernel->size());
    for(int &i : *kernel) kernel_weight += i;

    // Modified pixels
    std::vector<std::vector<int> > mod_pixels;
    for(int x = 0; x < image->step; x++) {
        std::vector<int> col;
        for(int y = 0; y < image->rows; y++) {
            col.push_back(0);
        }
        mod_pixels.push_back(col);
    }

    // Apply kernel to image
    int max = 0;
    int min = 0;
    for(int x = 0; x < image->step; x++) {
        for(int y = 0; y < image->rows; y++) {

            // Loop through kernel
            for(int i = 0; i < kernel->size(); i++) {

                // Find the pixel value at the correct offset
                int px_val = get_virtual_px_val(
                        image, 
                        x + ((i % kernel_dim) - (kernel_dim / 2)),
                        y + ((i / kernel_dim) - (kernel_dim / 2))
                        );
                mod_pixels[x][y] += kernel->at(i) * px_val;
            }
            if(kernel_weight != 0) mod_pixels[x][y] /= kernel_weight;
            //std::cout << "Output pix: " << mod_pixels[x][y] << std::endl;
            if(mod_pixels[x][y] > max) max = mod_pixels[x][y];
            if(mod_pixels[x][y] < min) min = mod_pixels[x][y];
        }
    }
    int range = max - min;

    // Write convoluted values to image
    for(int x = 0; x < image->step; x++) {
        for(int y = 0; y < image->rows; y++) {
            image->at<uchar>(y, x) = 255 * (mod_pixels[x][y] - min) / range;
        }
    }
}
void image_gaussian_blur(cv::Mat* image) {
    std::cout << "Blurring..." << std::endl;
    std::vector<int> kernel = {
        1, 2, 1,
        2, 4, 2,
        1, 2, 1
    };
    image_apply_kernel(image, &kernel);
}

void image_unsharp_masking(cv::Mat* image) {
    std::cout << "Unsharp masking..." << std::endl;
    cv::Mat blurred = image->clone();
    image_gaussian_blur(&blurred);

    *image = 2 * *image - blurred;

    for(int x = 0; x < image->step; x++) {
        for(int y = 0; y < image->rows; y++) {
            image->at<uchar>(y, x) = std::abs(int(image->at<uchar>(y, x)));
        }
    }
}

// Morphological Algos

void image_dilate(cv::Mat* image, std::vector<Point> strel) {
    std::cout << "Dilating..." << std::endl;

    std::vector<Point> dilation;

    // Loop through each pixel in image
    for(int y = 0; y < image->rows; y++) {
        for(int x = 0; x < image->step; x++) {

            bool dilate = false;

            // Loop through each point defined in strelture element
            for(auto p : strel) {

                int offsetX = x + p.x;
                int offsetY = y + p.y;

                // Check if offset of point to the current pixel is within bounds
                if(offsetX >= 0 && offsetX < image->step && offsetY >= 0 && offsetY < image->rows){

                    // If offset point is white, make current px gray
                    if(image->at<uchar>(offsetY, offsetX) == 255) dilate = true;
                }
            }
            if(dilate) dilation.push_back(Point{x, y});
        }
    }
    for(int y = 0; y < image->rows; y++) {
        for(int x = 0; x < image->step; x++) {
            image->at<uchar>(y, x) = 0;
        }
    }
    for(auto p : dilation) {
        image->at<uchar>(p.y, p.x) = 255;
    }
}

std::vector<std::vector<int> > image_dilate(std::vector<std::vector<int> > image, std::vector<Point> strel) {
    std::cout << "Dilating..." << std::endl;

    std::vector<Point> dilation;

    // Loop through each pixel in image
    for(int y = 0; y < image[0].size(); y++) {
        for(int x = 0; x < image.size(); x++) {

            bool dilate = false;

            // Loop through each point defined in strelture element
            for(auto p : strel) {

                int offsetX = x + p.x;
                int offsetY = y + p.y;

                // Check if offset of point to the current pixel is within bounds
                if(offsetX >= 0 && offsetX < image.size() && offsetY >= 0 && offsetY < image[0].size()){

                    // If offset point is white, make current px gray
                    if(image[offsetX][offsetY] > 0) dilate = true;
                }
            }
            if(dilate) dilation.push_back(Point{x, y});
        }
    }
    for(int y = 0; y < image[0].size(); y++) {
        for(int x = 0; x < image.size(); x++) {
            image[x][y] = 0;
        }
    }
    for(auto p : dilation) {
        image[p.x][p.y] = 1;
    }
    return image;
}

void image_erode(cv::Mat* image, std::vector<Point> strel) {
    std::cout << "Eroding..." << std::endl;

    std::vector<Point> erosion;

    // Loop through each pixel in image
    for(int y = 0; y < image->rows; y++) {
        for(int x = 0; x < image->step; x++) {

            bool erode = true;

            // Loop through each point defined in strelture element
            for(auto p : strel) {

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

std::vector<std::vector<int> > image_erode(std::vector<std::vector<int> > image, std::vector<Point> strel) {
    std::cout << "Eroding..." << std::endl;

    std::vector<Point> erosion;

    // Loop through each pixel in image
    for(int y = 0; y < image[0].size(); y++) {
        for(int x = 0; x < image.size(); x++) {

            bool erode = true;

            // Loop through each point defined in strelture element
            for(auto p : strel) {

                int offsetX = x + p.x;
                int offsetY = y + p.y;

                // Check if offset of point to the current pixel is within bounds
                if(offsetX >= 0 && offsetX < image.size() && offsetY >= 0 && offsetY < image[0].size()){

                    // If offset point is white, make current px gray
                    if(image[offsetX][offsetY] == 0) erode = false;
                }
            }
            if(erode) erosion.push_back(Point{x, y});
        }
    }
    for(int y = 0; y < image[0].size(); y++) {
        for(int x = 0; x < image.size(); x++) {
            image[x][y] = 0;
        }
    }
    for(auto p : erosion) {
        image[p.x][p.y] = 1;
    }
    return image;
}

void image_open(cv::Mat* image, std::vector<Point> strel) {
    image_erode(image, strel);
    image_dilate(image, strel);
}

std::vector<std::vector<int> > image_open(std::vector<std::vector<int> > image, std::vector<Point> strel) {
    image[0][0] = 999999999;
    return image_dilate(image_erode(image, strel), strel);
}

void image_close(cv::Mat* image, std::vector<Point> strel) {
    image_dilate(image, strel);
    image_erode(image, strel);
}

std::vector<std::vector<int> > image_close(std::vector<std::vector<int> > image, std::vector<Point> strel) {
    image = image_dilate(image, strel);
    image = image_erode(image, strel);
    return image;
}

std::vector<Point> gen_strel_cross(int size) {
    std::cout << "Generating a cross structural element of radius " << size << std::endl;
    std::vector<Point> strel;
    strel.push_back(Point{0,0});
    for(int i = 1; i <= size; i++) {
        strel.push_back(Point{0,i});
        strel.push_back(Point{0,-i});
        strel.push_back(Point{i,0});
        strel.push_back(Point{-i,0});
    }
    return strel;
}

std::vector<Point> gen_strel_square(int size) {
    std::cout << "Generating a square structural element of radius " << size << std::endl;
    std::vector<Point> strel;
    for(int i = -size; i <= size; i++) {
        for(int j = -size; j <= size; j++) {
            strel.push_back(Point{i,j});
        }
    }
    return strel;
}

// Expects binary image
std::vector<std::vector<int> > image_detect_regions(cv::Mat* image) {
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

    regions.erase(
        std::remove_if(
            regions.begin(), 
            regions.end(), 
            [](const std::vector<int> &a) {
                return a.size() < 20;
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

    std::cout << regions.size() << " regions detected!\n";

    for(int i = 1; i <= regions.size(); i++) {
        std::cout << "Region " << i << " has " << regions[i-1].size() << " pixels\n";
    }

    std::cout << "Smallest region: " << regions.front().size() << " | Largest region: " << regions.back().size() << std::endl;

    return regions;
}

// Expects color image
void image_highlight_regions(cv::Mat* image, std::vector<std::vector<int> > regions) {
}
