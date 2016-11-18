#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <fstream>

#include "driver.hpp"

const std::string GUI_NAME = "CS 455 Assignment 4";

int main(int argc, char **argv) {

    std::vector<std::string> image_names {
        // Image paths
        "./img/pigs.bmp"
    };

    std::vector<std::vector<cv::Mat> > images;
    for(int i = 0; i < image_names.size(); i++) {
        std::cout << "Currently loading " << image_names[i] << "...\n";
        cv::Mat image = cv::imread(image_names[i], CV_LOAD_IMAGE_COLOR);
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
        // Window names
        "Pigs"
    };

    for(int i = 0; i < window_names.size(); i++) {
        std::cout << "Creating window " << window_names[i] << "...\n";
        cv::namedWindow(window_names[i], cv::WINDOW_NORMAL);
    }
    
    int strel_size = 1;
    std::vector<Point> strel;
    strel.push_back(Point{0, 0});

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
            case 'g':   // Convert to grayscale
                for(auto &img : images) image_convert_grayscale(&img[1]);
                break;
            case 'r':   // Convert to rgb 
                for(auto &img : images) image_convert_color(&img[1]);
                break;
            case 'n':   // Negative
                for(auto &img : images) image_generate_negative(&img[1]);
                break;
            case 'D':   // Detect regions
                for(auto &img : images) image_detect_regions(&img[1]);
                break;
            case 'b':   // Binary
                for(auto &img : images) image_generate_binary(&img[1]);
                break;
            case 'B':   // Blur
                for(auto &img : images) image_gaussian_blur(&img[1]);
                break;
            case 'u':   // Unsharp mask
                for(auto &img : images) image_unsharp_masking(&img[1]);
                break;
            case 'd':   // Dilating
                for(auto &img : images) image_dilate(&img[1], strel);
                break;
            case 'e':   // Eroding
                for(auto &img : images) image_erode(&img[1], strel);
                break;
            case 'o':   // Opening
                for(auto &img : images) image_open(&img[1], strel);
                break;
            case 'c':   // Closing
                for(auto &img : images) image_close(&img[1], strel);
                break;
            case 'x':   // Cross strel
                strel = gen_strel_cross(strel_size);
                break;
            case 's':   // Square strel
                strel = gen_strel_square(strel_size);
                break;
            case '1':   // Resize strel setting
                strel_size = 1;
                std::cout << "Structural element size = 1" << std::endl;
                break;
            case '2':   // Resize strel setting
                strel_size = 2;
                std::cout << "Structural element size = 2" << std::endl;
                break;
            case '3':   // Resize strel setting
                strel_size = 3;
                std::cout << "Structural element size = 3" << std::endl;
                break;
            case '4':   // Resize strel setting
                strel_size = 4;
                std::cout << "Structural element size = 4" << std::endl;
                break;
            case '5':   // Resize strel setting
                strel_size = 5;
                std::cout << "Structural element size = 5" << std::endl;
                break;
            case '6':   // Resize strel setting
                strel_size = 6;
                std::cout << "Structural element size = 6" << std::endl;
                break;
            case '7':   // Resize strel setting
                strel_size = 7;
                std::cout << "Structural element size = 7" << std::endl;
                break;
            case '8':   // Resize strel setting
                strel_size = 8;
                std::cout << "Structural element size = 8" << std::endl;
                break;
            case '9':   // Resize strel setting
                strel_size = 9;
                std::cout << "Structural element size = 9" << std::endl;
                break;
            case 'p':   // Count pigs
                strel = gen_strel_square(2);
                for(auto &img : images) image_open(&img[1], strel);
                for(auto &img : images) image_close(&img[1], strel);
                for(auto &img : images) image_detect_regions(&img[1]);
                break;
            //case '':   // 
            //    for(auto &img : images) image_(&img[1]);
            //    break;
        }
    }
    cv::destroyAllWindows();
}
