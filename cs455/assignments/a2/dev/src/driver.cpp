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
        "Unsharp Mask 1",
        "Unsharp Mask 2",
    };

    for(int i = 0; i < window_names.size(); i++) {
        std::cout << "Creating window " << window_names[i] << "...\n";
        cv::namedWindow(window_names[i]);
    }
    
    bool run = true;
    bool modified = false;
    while(run) {
        for(int i = 0; i < window_names.size(); i++) {
            cv::imshow(window_names[i], images[i][1]);
        }
        char c = cvWaitKey(20);
        switch(c) {
            case 27:    // Escape
                run = false;
                break;
            case ' ':   // Toggle image modification
                if(modified) {
                    for(int i = 0; i < image_names.size(); i++) {
                        images[i][1] = images[i][0].clone();
                    }
                } else {
                    image_unsharp_masking(&images[0][1]);
                    image_unsharp_masking(&images[1][1]);
                }
                modified = !modified;    
                break;
        }
    }
    cv::destroyAllWindows();
}
