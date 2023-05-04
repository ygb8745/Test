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

int main() {
  std::string folder = "../"; // /path/to/folder
  std::string pattern = ".txt";

  std::vector<std::string> files;

  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(folder.c_str())) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      std::string filename(ent->d_name);
      if (filename.size() >= pattern.size() &&
          filename.substr(filename.size() - pattern.size()) == pattern) {
        files.push_back(folder + filename); // 将符合要求的文件路径添加到 files
      }
    }
    closedir(dir);
  } else {
    std::cout << "Could not open directory: " << folder << std::endl;
    return EXIT_FAILURE;
  }

  // 逐个读取文件
  for (const auto& file : files) {
    std::cout << "Reading file: " << file << std::endl;
    openFile(file);
    std::cout << std::endl;
  }

  return 0;
}
