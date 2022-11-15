#include <gtest/gtest.h>
#include "FolderTerrorist/FolderTerrorist.h"
#include <filesystem>
#include <fstream>

std::string arg0;
std::string foldername;
std::string init_filename;
std::string init_data;
std::vector<uint8_t> key;

std::string cd_to_updir(const std::string arg){
    int pos = arg.find_last_of('/');
    std::string buff = arg.substr(0,pos);
    return buff;
}

///unit tests

TEST(inst_testing, singleton_instance){
        FolderTerrorist *inst1 = FolderTerrorist::GetInstance();
        FolderTerrorist *inst2 = FolderTerrorist::GetInstance();
        ASSERT_EQ(inst1, inst2);
}

TEST(start_testing, start1){
    //create tmp folder for test
    std::string foldername = cd_to_updir(cd_to_updir(arg0)) + "/test";
    std::filesystem::create_directory(foldername);
    //testing
    FolderTerrorist *inst = FolderTerrorist::GetInstance();
    inst->SetFolderName(foldername);
    inst->SetTheBorder(1);
    EXPECT_NO_THROW({
        inst->Start("encryption");
    });
    std::filesystem::remove_all(foldername);
}

TEST(start_testing, start2){
    //create tmp folder for test
    std::string foldername = cd_to_updir(cd_to_updir(arg0)) + "/test";
    std::filesystem::create_directory(foldername);
    //testing
    FolderTerrorist *inst = FolderTerrorist::GetInstance();
    inst->SetTheBorder(1);
    EXPECT_NO_THROW({
        inst->Start(foldername, "decryption");
    });
    std::filesystem::remove_all(foldername);
}

TEST(start_testing, start_exeption1){
    FolderTerrorist *inst = FolderTerrorist::GetInstance();
    inst->SetTheBorder(1);
    EXPECT_ANY_THROW({
        inst->Start("encryption");
    });
}

TEST(start_testing, start_exeption2){
    //create tmp folder for test
    std::string foldername = cd_to_updir(cd_to_updir(arg0)) + "/test";
    std::filesystem::create_directory(foldername);
    //testing
    FolderTerrorist *inst = FolderTerrorist::GetInstance();
    inst->SetFolderName(foldername);
    inst->SetTheBorder(1);
    EXPECT_ANY_THROW({
        inst->Start("go do crypto");
    });
    std::filesystem::remove_all(foldername);
}

TEST(keygen_testing, get_key){
    FolderTerrorist *inst = FolderTerrorist::GetInstance();
    //generating default key by GetKey()
    auto key = inst->GetKey();
    EXPECT_TRUE(key.size() == 15);
}

TEST(keygen_testing, keygen){
    FolderTerrorist *inst = FolderTerrorist::GetInstance();
    auto key = keygen(30);
    inst->SetKey(key);
    EXPECT_TRUE(inst->GetKey().size() == 30);
}

TEST(terror_testing_group, folder_files_encryption){
    //create tmp folder for test group
    std::filesystem::remove_all(foldername);
    std::filesystem::create_directory(foldername);
    //create test group file with initial data
    std::ofstream ofs(init_filename);
    ofs << init_data;
    ofs.close();

    FolderTerrorist *inst = FolderTerrorist::GetInstance();
    inst->SetTheBorder(1);

    //testing XOR encryption
    inst->SetKey(key);
    inst->Start(foldername, "encryption");

    //get result file data
    std::string encrypted_data;
    std::ifstream ifs(init_filename + ".terror");
    ifs >> encrypted_data;
    ifs.close();

    ASSERT_NE(init_data, encrypted_data);
}

TEST(terror_testing_group, folder_files_decryption){
    FolderTerrorist *inst = FolderTerrorist::GetInstance();
    inst->SetTheBorder(1);

    //testing XOR decryption
    inst->SetKey(key);
    inst->Start(foldername, "decryption");

    //get result file data
    std::string decrypted_data;
    std::ifstream ifs(init_filename);
    char buff_cr;
    while(ifs.get(buff_cr)) decrypted_data.push_back(buff_cr);
    ifs.close();

    std::filesystem::remove_all(foldername);
    ASSERT_EQ(init_data, decrypted_data);
}

TEST(signals_testing, catching){
    FolderTerrorist *inst = FolderTerrorist::GetInstance();
    inst->SignalsCatching();
    ASSERT_NO_FATAL_FAILURE({std::raise(5);});
}

int main(int argc, char *argv[]) {
    //common initial data
    arg0 = argv[0];
    init_data = "VERY NECESSARY INFORMATION!!!!";
    foldername = cd_to_updir(cd_to_updir(arg0)) + "/test";
    init_filename = foldername + "/tmp.data";
    key = keygen(15);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}