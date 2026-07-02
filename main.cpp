#include <filesystem>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <algorithm>

namespace fs = std::filesystem;

int main()
{
    // configuration
    std::string inputPath;

    std::cout << "=======================================" << std::endl;
    std::cout << "    FILE ORGANIZER - CONFIGURATION     " << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "Enter the absolute path of the folder to organize \n";
    std::cout << "(Or press ENTER to use your system's default Downloads folder): ";

    std::getline(std::cin, inputPath);

    if (inputPath.empty())
    {
        std::string homeDir;

        #if defined(_WIN32) || defined(_WIN64)
        // windows
            const char* userProfile = getenv("USERPROFILE");
            if (userProfile) homeDir = userProfile;
            inputPath = homeDir + "\\Downloads";

        #else
        // macos/linux
            const char* home = std::getenv("HOME");
            if (home) homeDir = home;
            inputPath = homeDir + "/Downloads";

        #endif
    }

    fs::path targetFolder(inputPath);

    if (!fs::exists(targetFolder))
    {
        std::cerr << "\n[ERROR] The folder '" << targetFolder.string() << "' does not exist!" << std::endl;
        return 1;
    }

    // --------------

    const std::unordered_map<std::string, std::unordered_set<std::string>> categories = {
        {"documents",  {".pdf", ".docx", ".doc", ".txt", ".rtf", ".odt"}},
        {"images",     {".jpg", ".jpeg", ".png", ".gif", ".webp", ".svg", ".bmp"}},
        {"archives",   {".zip", ".tar", ".gz", ".7z", ".rar", ".bz2", ".tgz"}},
        {"installers", {".dmg", ".pkg", ".exe", ".msi"}},
        {"code",       {".cpp", ".h", ".py", ".java", ".html", ".css", ".js", ".json", ".sh"}},
        {"media",      {".mp3", ".mp4", ".m4a", ".mkv", ".mov", ".avi"}}
    };

    std::cout << "--- Folder: " << targetFolder << " ---" << std::endl;

    // go through the whole directory and print every file that's in it
    for (const auto& element : fs::directory_iterator(targetFolder))
    {
        if (fs::is_regular_file(element))
        {
            std::cout << "[FILE] " << element.path().filename().string() << std::endl;

            fs::path fileExtension = element.path().extension();
            fs::path filePath = element.path();
            fs::path fileName = element.path().filename();

            // convert file extensions to lower case
            std::string extStr = fileExtension.string();
            std::transform(extStr.begin(), extStr.end(), extStr.begin(), ::tolower);

            for (const auto& pair : categories)
            {
                const std::string& folderName = pair.first;
                const std::unordered_set<std::string>& extensions = pair.second;

                if (extensions.contains(extStr))
                {
                    try
                    {
                        fs::path destinationFolder = targetFolder / folderName;

                        fs::create_directory(destinationFolder);
                        fs::path newPath = destinationFolder / fileName;
                        fs::rename(filePath, newPath);

                        std::cout << "[MOVED] " << fileName << " to " << folderName << std::endl;

                        break;
                    }
                    catch (const fs::filesystem_error& e)
                    {
                        std::cerr << "[ERROR] Unable to move file: " << fileName << " to " << folderName << "\nReason: " << e.what() << std::endl;
                    }
                }
            }
        }
    }


    std::cout << "\n--- DONE ---\n";
    return 0;
}

