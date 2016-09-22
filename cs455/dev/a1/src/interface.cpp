#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <fstream>

#include "interface.hpp"

const std::string GUI_NAME = "CS 455 Assignment 1";

int main(int argc, char **argv) {
    //if(argc != 2) {
    //    fprintf(stderr, "Expecting a .tif image as an argument.\n");
    //    return 1;
    //}

    std::vector<std::string> image_names {
        "img/night.tif",
        "img/night.tif",
        "img/nyc.tif",
        "img/shapes.tif",
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
        "Binary Transformation 2",
    };

    image_generate_negative(&images[0][1]);
    image_histogram_equalize(&images[1][1]);
    image_histogram_equalize(&images[2][1]);
    image_generate_binary(&images[3][1]);
    image_generate_binary(&images[4][1]);

    for(int i = 0; i < window_names.size(); i++) {
        std::cout << "Creating window " << window_names[i] << "...\n";

        cv::namedWindow("Before " + window_names[i]);
        cv::namedWindow("After " + window_names[i]);
        
        std::vector<int> hist_before = image_histogram(&images[i][0]);
        std::vector<int> hist_after = image_histogram(&images[i][1]);

        std::ofstream out_file("data/" + window_names[i] +".csv");
        for(int k = 0; k < 256; k++) {
            //printf("%3i: %6i\n", i, hist[i]);
            out_file << k << "," << hist_before[k] << "," << hist_after[k] << std::endl;
        }
        out_file.close();
    }


        
    bool run = true;
    while(run) {
        for(int i = 0; i < window_names.size(); i++) {
            cv::imshow("Before " + window_names[i], images[i][0]);
            cv::imshow("After " + window_names[i], images[i][1]);
        }

        char c = cvWaitKey(20);

        switch(c) {
            case 27:
                run = false;
                break;
        }
    }
    cv::destroyAllWindows();
}
