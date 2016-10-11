#include "driver.hpp"
#include <queue>
#include <iostream>
#include <algorithm>

int image_total_pixels(cv::Mat *image) {
    return image->rows * image->step; 
}

void image_generate_negative(cv::Mat* image) {
    int total_pixels = image_total_pixels(image);
    for(int i = 0; i < total_pixels; i++) {
        image->data[i] = 255 - image->data[i];
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
    //std::cout << "Applying kernel..." << std::endl;

    int kernel_weight = 0;
    int kernel_dim = std::floor(std::sqrt(kernel->size()));
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
}

void image_sobel_operator(cv::Mat* image) {
    std::cout << "Applying Sobel..." << std::endl;

    cv::Mat horiz_image = image->clone();
    std::vector<int> horiz_kernel = {
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1
    };
    image_apply_kernel(&horiz_image, &horiz_kernel);

    cv::Mat vert_image = image->clone();
    std::vector<int> vert_kernel = {
        -1,  0,  1,
        -2,  0,  2,
        -1,  0,  1
    };
    image_apply_kernel(&vert_image, &vert_kernel);

    cv::absdiff(horiz_image, vert_image, *image);
}

std::vector<int> laplacian_gaussian_mask(int, double) {
    std::vector<int> mask;
    return mask;
}
