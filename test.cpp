#include <gtest/gtest.h>
#include "FolderTerrorist/FolderTerrorist.h"
#include <filesystem>
#include <fstream>

std::string arg0;
std::string cd_to_updir(const std::string arg){
    int pos = arg.find_last_of('/');
    std::string buff = arg.substr(0,pos);
    return buff;
}

///unit tests
TEST(terror_testing, singleton_instance){
        FolderTerrorist *inst1 = FolderTerrorist::GetInstance();
        FolderTerrorist *inst2 = FolderTerrorist::GetInstance();
        ASSERT_EQ(inst1, inst2);
}

TEST(terror_testing, start1){
    std::string foldername = cd_to_updir(cd_to_updir(arg0)) + "/test";
    std::filesystem::create_directory(foldername);
    FolderTerrorist *inst = FolderTerrorist::GetInstance();
    inst->SetFolderName(foldername);
    inst->SetTheBorder(1);
    EXPECT_NO_THROW({
        inst->Start();
    });
    std::filesystem::remove_all(foldername);
}

TEST(terror_testing, start2){
    std::string foldername = cd_to_updir(cd_to_updir(arg0)) + "/test";
    std::filesystem::create_directory(foldername);
    FolderTerrorist *inst = FolderTerrorist::GetInstance();
    inst->SetTheBorder(1);
    EXPECT_NO_THROW({
        inst->Start(foldername);
    });
    std::filesystem::remove_all(foldername);
}

TEST(terror_testing, folder_files_terror){
    std::string foldername = cd_to_updir(cd_to_updir(arg0)) + "/test";
    std::filesystem::create_directory(foldername);

    std::string init_data = "1234567890";
    std::fstream ofs;
    std::string filename = foldername + "/tmp.data";
    ofs.open(filename);
    ofs << init_data;
    ofs.close();

    FolderTerrorist *inst = FolderTerrorist::GetInstance();
    inst->SetTheBorder(1);
    inst->Start(foldername);

    std::string terror_data;
    std::fstream ifs;
    ifs.open(filename + ".terror");
    ifs >> terror_data;
    EXPECT_TRUE(!is_regular_file(std::filesystem::path(filename)) && init_data != terror_data);
    std::filesystem::remove_all(foldername);
}

TEST(terror_testing, signals_catching){
    FolderTerrorist *inst = FolderTerrorist::GetInstance();
    inst->SignalsCatching();
    ASSERT_NO_FATAL_FAILURE({std::raise(5);});
}

int main(int argc, char *argv[]) {
    arg0 = argv[0];
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}