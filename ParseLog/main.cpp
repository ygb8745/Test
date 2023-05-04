#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>

#define CSV_HEAD "time,gep_name,counter,number\n"

struct ic_file{
    std::string full_path;
    std::string gep_name; // also folder name.
    //
    ic_file(std::string full_path, std::string gep_name):full_path(full_path), gep_name(gep_name){}
};

struct counter_entity{
    std::string date_time;
    std::string gep_name;
    std::string name;
    int num;
};

std::vector<counter_entity> all_counters;
std::string out_put_file_path;

// std::regex any_line_pattern("=====");
// std::regex equal_sign_divider_pattern("=====");
std::regex time_pattern("^Time:\\w+(\\d+-\\d+-\\d+)\\s+(\\d+:\\d+:\\d+)");
std::regex ic_pattern("^ic:\\s+((\\d+\\s?))");
std::regex ic_labels_pattern("ic-labels:\\s+((\\w+,?))");

std::vector<std::string> split(const std::string &str, const std::string &pattern)
{
    std::vector<std::string> res;
    if(str == "")
        return res;
    //在字符串末尾也加入分隔符，方便截取最后一段
    std::string strs = str + pattern;
    size_t pos = strs.find(pattern);

    while(pos != strs.npos)
    {
        std::string temp = strs.substr(0, pos);
        res.push_back(temp);
        //去掉已分割的字符串,在剩下的字符串中进行分割
        strs = strs.substr(pos+1, strs.size());
        pos = strs.find(pattern);
    }

    return res;
}

void openFile(ic_file icFile) {
    std::ifstream file(icFile.full_path);
    if (!file.is_open()) {
        std::cout << "Could not open file to read: " << icFile.full_path << std::endl;
        return;
    }

    // 打开已存在的文件并找到文件末尾
    std::ofstream out(out_put_file_path, std::ios_base::app);
    if (!out.is_open()) {
        std::cout << "Could not open file to write: " << out_put_file_path << std::endl;
        return;
    }

    std::vector<std::string> all_lines;

    // 读取文件内容
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
        all_lines.push_back(line);
    }
    std::smatch res1;
    std::smatch res2;
    std::smatch res3;
    for(int i=0;i<all_lines.size(); i++){
        if(i+6<all_lines.size() &&(i==0 || all_lines[i] == "=====")){
            if(std::regex_match(all_lines[i+1], res1, time_pattern)
                    &&all_lines[i+2] == "-"
                    &&std::regex_match(all_lines[i+3], res2, ic_pattern)
                    &&std::regex_match(all_lines[i+6], res3, ic_labels_pattern) ){
                std::vector<std::string> num_string_list = split(res2.str(1), " ");
                std::vector<std::string> counter_names_list = split(res3.str(1), " ");
                if(num_string_list.size() != counter_names_list.size()){
                    std::cout << "Number of id and ic_labels miss match:"
                            "\nFile: " << icFile.full_path <<
                            "\nLine:" << i << "\n";
                }else{
                    for(int j=0;j<num_string_list.size(); j++){
                        counter_entity counter_tmp;
                        counter_tmp.date_time = res1.str(1) + res1.str(2);
                        counter_tmp.gep_name = icFile.gep_name;
                        counter_tmp.name=counter_names_list[j];
                        counter_tmp.num=stoi(num_string_list[j]);
                        all_counters.push_back(counter_tmp);
                        out <<counter_tmp.date_time
                            <<"," << counter_tmp.gep_name
                            <<"," << counter_tmp.name
                            <<"," << counter_tmp.num << std::endl;
                    }
                }
                i += 6;
            }
        }
    }

    // 关闭文件
    file.close();
    out.close();

}

void findFiles(std::string folder, std::string pattern, std::vector<ic_file>& files) {
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
                files.push_back(ic_file(std::string(folder + "/" + filename), folder)); // 将符合要求的文件路径添加到 files
            }
        }
        closedir(dir);
    }
    else {
        std::cout << "Could not open directory: " << folder << std::endl;
        return;
    }
}

void clearFile(std::string fileName) {
    // 创建一个输出流
    std::ofstream out(fileName, std::ofstream::out | std::ofstream::trunc);

    // 关闭输出流
    out.close();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " folder_path\n";
        return EXIT_FAILURE;
    }
    if(argc >=3){
        out_put_file_path = argv[2];
    }else{
        out_put_file_path = "all_internal_counters.csv";
    }
    clearFile(out_put_file_path);
    std::ofstream out(out_put_file_path, std::ios_base::app);

    if (!out.is_open()) {
        std::cout << "Could not open file: " << out_put_file_path << std::endl;
        return 11;
    }

    // 写入文件内容
    out << CSV_HEAD << std::endl;

    // 关闭文件
    out.close();

    std::string folder = argv[1];
    std::string pattern = ".txt";

    std::vector<ic_file> files;
    findFiles(folder, pattern, files);

    // 逐个读取文件
    for (const auto& file : files) {
        std::cout << "Reading file: " << file.full_path << std::endl;
        openFile(file);
        std::cout << std::endl;
    }

    std::cout << "all_counters.size : " << all_counters.size() << "\n";

    return EXIT_SUCCESS;
}
