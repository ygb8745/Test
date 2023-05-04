#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

void openFile(std::string fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cout << "Could not open file: " << fileName << std::endl;
        return;
    }

    // 读取文件内容
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }

    file.close();
}

void findFiles(std::string folder, std::string pattern, std::vector<std::string>& files) {
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(folder.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            std::string filename(ent->d_name);
            if (filename == "." || filename == "..") {
                continue; // 忽略当前目录和父目录
            }
            else if (ent->d_type == DT_DIR) {
                std::string subdir = folder + "/" + filename;
                findFiles(subdir, pattern, files); // 递归调用 findFiles 函数，处理子目录
            }
            else if (filename.size() >= pattern.size() && filename.substr(filename.size() - pattern.size()) == pattern) {
                files.push_back(folder + "/" + filename); // 将符合要求的文件路径添加到 files
            }
        }
        closedir(dir);
    }
    else {
        std::cout << "Could not open directory: " << folder << std::endl;
        return;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " folder_path\n";
        return EXIT_FAILURE;
    }

    std::string folder = argv[1];
    std::string pattern = ".txt";

    std::vector<std::string> files;
    findFiles(folder, pattern, files);

    // 逐个读取文件
    for (const auto& file : files) {
        std::cout << "Reading file: " << file << std::endl;
        openFile(file);
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
