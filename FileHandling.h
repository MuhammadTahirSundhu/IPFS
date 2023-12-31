#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include  <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include "sha1.hpp"

#ifdef _WIN32
#include <direct.h>  // For Windows
#else
#include <sys/stat.h>  // For Unix-like systems
#endif
bool deleteFile1(const std::string& filePath)
{
    int result = std::remove(filePath.c_str());

    if (result == 0) {
        std::cout << "File deleted successfully." << std::endl;
        return true;
    }
    else {
        std::cerr << "Failed to delete file!" << std::endl;
        return false;
    }
}
std::string readFilevedio(const std::string& filePath, bool& status) {
    cv::VideoCapture cap(filePath);

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open or find the MP4 file " << filePath << std::endl;
        status = false;
        return "";
    }

    cv::Mat frame;
    std::ostringstream videoContent;

    while (cap.read(frame)) {
        // Convert each frame to grayscale and append to the content
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        videoContent << frame;
        status = true;
    }

    return videoContent.str();
}

std::string readFilewav(const std::string& filePath, bool& status) {
    //  Read a WAV file using OpenCV
    cv::Mat audio = cv::imread(filePath, cv::IMREAD_UNCHANGED);

    if (audio.empty()) {
        std::cerr << "Error: Could not open or find the WAV file " << filePath << std::endl;
        status = false;
        return "";
    }

    //  Assuming the audio is in 16-bit signed PCM format
    if (audio.depth() != CV_16S) {
        std::cerr << "Error: Unsupported audio format. Only 16-bit signed PCM is supported." << std::endl;
        status = false;
        return "";
    }

    //  Convert the audio matrix to a string
    std::string audioContent(reinterpret_cast<const char*>(audio.data), audio.total() * audio.channels() * sizeof(short));
    status = true;
    return audioContent;
}

std::string readFiletxt(const std::string& filePath, bool& status) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open or find the file " << filePath << std::endl;
        status = false;;
        return "";
    }

    std::ostringstream content;
    content << file.rdbuf();
    status = true;
    return content.str();
}

std::string readImagejpg(const std::string& filePath, bool& status) {
    //   Extract file extension from the file path
    size_t dotPos = filePath.find_last_of('.');
    if (dotPos == std::string::npos) {
        std::cerr << "Error: File path does not contain a valid file extension." << std::endl;
        status = false;
        return "";
    }

    std::string fileExtension = filePath.substr(dotPos + 1);

    // Read an image from file
    cv::Mat image = cv::imread(filePath);

    if (image.empty()) {
        std::cerr << "Error: Could not open or find the image " << filePath << std::endl;
        status = false;;
        return "";
    }

    std::vector<uchar> buffer;
    cv::imencode("." + fileExtension, image, buffer);

    std::string imageContent(buffer.begin(), buffer.end());
    status = true;
    return imageContent;
}

std::string readFileImage(const std::string& filePath, bool& status) {
    size_t dotPos = filePath.find_last_of('.');
    if (dotPos == std::string::npos) {
        std::cerr << "Error: File path does not contain a valid file extension." << std::endl;
        status = false;
        return "";
    }

    std::string fileExtension = filePath.substr(dotPos + 1);

    cv::Mat image = cv::imread(filePath);

    if (image.empty()) {
        std::cerr << "Error: Could not open or find the image " << filePath << std::endl;
        status = false;
        return "";
    }

    std::vector<uchar> buffer;
    cv::imencode("." + fileExtension, image, buffer);

    std::string imageContent(buffer.begin(), buffer.end());
    status = true;
    return imageContent;
}


string getfileContent(string path, bool& status)
{
    string filePath = path;
    if (path.length() < 3) {
        status = false;
        return "";
    }

    string fileExtension = "";
    int i = filePath.length();
    while (filePath[i] != '.') {
        fileExtension = filePath[i] + fileExtension;
        i--;
    }

   
    string fileContent = "";
    bool textEntesion1 = true;
    string textextenion = "txt";
    for (int i = 0; i < 3; i++) {
        if (fileExtension[i] != textextenion[i])
            textEntesion1 = false;
    }
   
    
    if (textEntesion1) {
        fileContent = readFiletxt(filePath, status);
        return fileContent;
    }
    bool textEntesion2 = true;
    if (!textEntesion1) {
        textextenion = "mp4";
        for (int i = 0; i < 3; i++) {
            if (fileExtension[i] != textextenion[i])
                textEntesion2 = false;
        }
    }

    if (textEntesion2) {
        fileContent = readFilevedio(filePath, status);
        return fileContent;
    }
  
   
    fileContent = readFileImage(filePath, status);
    

    if (fileContent.empty()) {
        std::cerr << "Error reading file or unsupported file type." << std::endl;
        status = false;
    }
    return fileContent;
}


///////////////////////////////////////////////
bool deleteDirectory(const std::string& path) {
#ifdef _WIN32
    int result = _rmdir(path.c_str());
#else
    int result = rmdir(path.c_str());
#endif

    if (result == 0) {
       // std::cout << "Directory deleted successfully." << std::endl;
        return true;
    }
    else {
       // std::cout << "Failed to delete directory!" << std::endl;
        return false;
    }
}

bool createDirectory(const std::string& path) {
#ifdef _WIN32
    int result = _mkdir(path.c_str());
#else
    int result = mkdir(path.c_str(), 0777);
#endif

    if (result == 0) {
       // std::cout << "Directory created successfully." << std::endl;
        return true;
    }
    else {
       // std::cout << "Failed to create directory!" << std::endl;
        return false;
    }
}

std::string writeImage(const std::string& imageContent, const std::string& filePath)
{
    // Convert the string to a uchar vector
    std::vector<uchar> buffer(imageContent.begin(), imageContent.end());

    // Decode the uchar vector to an image
    cv::Mat image = cv::imdecode(buffer, cv::IMREAD_COLOR);

    if (image.empty()) {
        std::cerr << "Error: Could not decode the image content" << std::endl;
        return "";
    }

    // Write the image to file
    cv::imwrite(filePath, image);

    return filePath;
}

bool writeFile(const std::string& filePath, const std::string& content) {
    std::ofstream file(filePath);


    string pt = writeImage(content, filePath);

    if (file.is_open()) {
        //  file << content;
        file.close();
        std::cout << "File created successfully at path: " << filePath << std::endl;
        return true;
    }
    else {
        std::cout << "Failed to create file at path: " << filePath << std::endl;
        return false;
    }
}

bool writeTextFile(const std::string& filePath, const std::string& content) {
    std::ofstream file(filePath);

    if (file.is_open()) {
        file << content;
        file.close();
        std::cout << "Text file created successfully at path: " << filePath << std::endl;
        return true;
    }
    else {
        std::cout << "Failed to create text file at path: " << filePath << std::endl;
        return false;
    }
}