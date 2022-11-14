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

    //~FolderTerrorist();

    void SetFolderName(const std::string &folderName);

    const std::string &GetFolderName() const;

    void Start();

    void Start(std::string path);

    void SetTheBorder(int border);

    void SignalsCatching();
private:
    FolderTerrorist();

    static inline FolderTerrorist *p_instance;

    std::string folder_name;

    std::vector<std::thread> threads;

    std::list<std::string> files_queue;

    std::mutex mtx;

    std::vector<std::string> GetFolderFiles();

    void UpdateQueue();

    void ReleaseQueue();

    void FolderFileTerror(const std::string &file_path);

    int border = -1;

    static void SignalsHandler(int signal);
};