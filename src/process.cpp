#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <vector>
#include <atomic>
#include <future>
#include <set>


bool is_valid_image_extension(const std::filesystem::path& p) {
    std::string ext = p.extension().string();
    return (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".bmp" || ext == ".tiff" || ext == ".JPG");
}


std::pair<int, cv::Mat> create_mask_and_count(const std::string& img_path) {
    std::cout << "Processing " << img_path << " on thread " << std::this_thread::get_id() << std::endl;

    // Read the image
    cv::Mat img = cv::imread(img_path);
    if (img.empty()) {
        std::cerr << "Error: Unable to read image " << img_path << std::endl;
        return {0, cv::Mat()};
    }

    // Create a mask where all 3 channels are above 200 (i.e., pixel values > 200)
    cv::Mat mask = (img > 200);  // This creates a binary mask, where pixels > 200 become 1, otherwise 0

    // Convert the mask to a single-channel image (grayscale) for countNonZero
    cv::Mat single_channel_mask;
    if (mask.channels() > 1) {
        cv::cvtColor(mask, single_channel_mask, cv::COLOR_BGR2GRAY);
    } else {
        single_channel_mask = mask;
    }

    // Count non-zero pixels (those satisfying the "max" condition)
    int max_pixel_count = cv::countNonZero(single_channel_mask);

    // Save the mask image (lossless format like PNG)
    std::string output_path = img_path + "_mask.png";
    cv::imwrite(output_path, single_channel_mask);

    std::cout << "Processed image, found " << max_pixel_count << " max pixels." << std::endl;

    return {max_pixel_count, single_channel_mask};
}

void process_images_in_parallel(const std::vector<std::string>& image_paths, std::atomic<int>& total_max_pixels) {
    std::vector<std::future<std::pair<int, cv::Mat>>> futures;


    for (const auto& img_path : image_paths) {
        futures.push_back(std::async(std::launch::async, create_mask_and_count, img_path));
    }

  
    for (auto& fut : futures) {
        auto [max_pixel_count, mask] = fut.get();
        total_max_pixels += max_pixel_count;
    }
}

int main() {

    std::string project_folder = std::filesystem::current_path().string();
    std::string image_folder = project_folder + "/data"; // Image folder within the src directory

    
    std::set<std::string> image_paths_set;

    
    for (const auto& entry : std::filesystem::directory_iterator(image_folder)) {
        std::string file_path = entry.path().string();

        // Only add valid image files that are not masks (based on extension)
        if (entry.is_regular_file() && is_valid_image_extension(entry.path()) && file_path.find("_mask.png") == std::string::npos) {
            image_paths_set.insert(file_path);
        }
    }

    std::vector<std::string> image_paths(image_paths_set.begin(), image_paths_set.end());
    
    std::atomic<int> total_max_pixels(0);

    process_images_in_parallel(image_paths, total_max_pixels);

    std::cout << "Total max pixels across all images: " << total_max_pixels.load() << std::endl;

    return 0;
}
