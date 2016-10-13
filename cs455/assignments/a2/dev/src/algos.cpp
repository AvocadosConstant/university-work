#include "driver.hpp"
#include <queue>
#include <iostream>
#include <algorithm>
#include <iomanip>

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

void print_square_matrix(std::vector<int> matrix) {
    int dim = std::sqrt(matrix.size());
    for(int i = 0; i < matrix.size(); i++) {
        std::cout << matrix[i] << "\t";
        if(i % dim == dim - 1) std::cout << std::endl;
    }
}

void image_apply_kernel(cv::Mat* image, std::vector<int>* kernel) {
    //std::cout << "Applying kernel..." << std::endl;

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

std::vector<int> laplacian_gaussian_mask(int dim, double sigma) {
    std::cout << "Generating the Laplacian of Gaussian Mask of size " 
        << dim << "x" << dim << " and sigma = " << sigma << "." << std::endl;
    const double PI  =3.141592653589793238463;
    double min = 9999999999;

    std::vector<int> mask;
    std::vector<double> raw_mask;
    double sigma_2 = sigma * sigma;
    double left_mult = 0.5 / (PI * sigma_2 * sigma_2);
    
    double sum = 0;
    for(int y = -dim / 2; y <= dim / 2; y++) {
        for(int x =  -dim / 2; x <= dim / 2; x++) {
            float val = 0;
    
            double x2y2sig2 = (x * x + y * y) / (sigma_2);
            val = left_mult * (x2y2sig2 - 2) * std::exp(-0.5 * x2y2sig2);

            sum += val;
            raw_mask.push_back(val);
            if(std::abs(val) < min && std::abs(val) > 0) min = std::abs(val);
        }
    }

    std::cout << "Raw mask values sum to " << sum << std::endl;

    for(double &d : raw_mask) {
        d /= min;
        mask.push_back(std::round(d));
    }
    
    //sum = 0;
    //for(double &d : raw_mask) {
    //    sum += d;
    //}
    //std::cout << "Processed mask values sum to " << sum << std::endl;
    return mask;
}
