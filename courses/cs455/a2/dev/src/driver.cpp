#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <fstream>

#include "driver.hpp"

const std::string GUI_NAME = "CS 455 Assignment 1";

int main(int argc, char **argv) {

    std::vector<std::string> image_names {
        "img/ant_gray.tif",
        "img/basel_gray.tif",
    };

    std::vector<std::vector<cv::Mat> > images;
    for(int i = 0; i < image_names.size(); i++) {
        std::cout << "Currently loading " << image_names[i] << "...\n";
        cv::Mat image = cv::imread(image_names[i], cv::IMREAD_GRAYSCALE);
        std::vector<cv::Mat> loaded_images {
            image,
            image.clone()
        };
        if(!loaded_images[0].data || !loaded_images[1].data) {
            fprintf(stderr, "Image loading failure\n");
            return 1;
        }
        images.push_back(loaded_images); 
    }

    std::vector<std::string> window_names {
        "Ant",
        "Basel",
    };

    for(int i = 0; i < window_names.size(); i++) {
        std::cout << "Creating window " << window_names[i] << "...\n";
        cv::namedWindow(window_names[i]);
    }
    
    bool run = true;
    while(run) {
        for(int i = 0; i < window_names.size(); i++) {
            cv::imshow(window_names[i], images[i][1]);
        }
        char c = cvWaitKey(20);
        std::vector<int> log_mask;
        switch(c) {
            case 27:    // Escape
                run = false;
                break;
            case ' ':   // Revert to original
                for(auto &img : images) img[1] = img[0].clone();
                break;
            case 'n':   // Negate
                for(auto &img : images) image_generate_negative(&img[1]);
                break;
            case 'b':   // Gaussian Blur 
                for(auto &img : images) image_gaussian_blur(&img[1]);
                break;
            case 'u':   // Unsharp Mask
                for(auto &img : images) image_unsharp_masking(&img[1]);
                break;
            case 's':   // Sobel Operator
                for(auto &img : images) image_sobel_operator(&img[1]);
                break;
            case 'l':   // Generate Laplacian of Gaussian Mask size 7
                log_mask = laplacian_gaussian_mask(7, 1.4);
                print_square_matrix(log_mask);
                for(auto &img : images) image_apply_kernel(&img[1], &log_mask);
                break;
            case 'L':   // Generate Laplacian of Gaussian Mask size 11
                log_mask = laplacian_gaussian_mask(11, 5.0);
                print_square_matrix(log_mask);
                for(auto &img : images) image_apply_kernel(&img[1], &log_mask);
                break;
        }
    }
    cv::destroyAllWindows();
}
