#include "FolderTerrorist/FolderTerrorist.h"

int main(int argc, char *argv[]) {
    FolderTerrorist *apocalypse = FolderTerrorist::GetInstance();
    apocalypse->SignalsCatching();
    apocalypse->Start(argv[1]);
    return 0;
}