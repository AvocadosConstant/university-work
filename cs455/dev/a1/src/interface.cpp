#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <fstream>

#include "interface.hpp"

const std::string GUI_NAME = "CS 455 Assignment 1";

int main(int argc, char **argv) {

    std::vector<std::string> image_names {
        "img/night.tif",
        "img/night.tif",
        "img/nyc.tif",
        "img/shapes.tif",
        "img/shapes.tif",
        "img/dontpanic.tif",
        "img/dontpanic.tif",
        //"img/paper.tif"
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
        "Negative Transformation",
        "Histogram Equalization 1",
        "Histogram Equalization 2",
        "Binary Transformation 1",
        "Region Detection 1",
        "Binary Transformation 2",
        "Region Detection 2",
    };

    image_generate_binary(&images[3][1]);
    images[4][0] = images[3][1].clone();
    images[4][1] = images[4][0].clone();

    image_generate_binary(&images[5][1]);
    images[6][0] = images[5][1].clone();
    images[6][1] = images[6][0].clone();

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
                for(int i = 0; i < image_names.size(); i++) {
                    if(!modified) {
                        images[i][1] = images[i][0].clone();
                    } else {
                        image_generate_negative(&images[0][1]);
                        image_histogram_equalize(&images[1][1]);
                        image_histogram_equalize(&images[2][1]);

                        image_generate_binary(&images[3][1]);
                        //image_detect_regions(&images[4][1]);

                        image_generate_binary(&images[5][1]);
                        //image_detect_regions(&images[6][1]);
                    }
                }
                modified = !modified;    
                break;
            case 'h':   // Generate histograms
                printf("Generating histograms...\n");
                for(int i = 0; i < window_names.size(); i++) {
                    std::vector<int> hist_before = image_histogram(&images[i][0]);
                    std::vector<int> hist_after = image_histogram(&images[i][1]);

                    std::string file_name = "data/" + window_names[i] +".csv";
                    std::cout << "\tWriting to " << file_name << "...\n";
                    std::ofstream out_file(file_name);

                    for(int k = 0; k < 256; k++) {
                        //printf("%3i: %6i\n", i, hist[i]);
                        out_file << k << "," << hist_before[k] << "," << hist_after[k] << std::endl;
                    }
                    out_file.close();
                }
                break;
        }
    }
    cv::destroyAllWindows();
}
