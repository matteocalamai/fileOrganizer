#include <filesystem>
#include <iostream>
#include <unordered_set>

namespace fs = std::filesystem;

int main()
{
    // define the path of the folder that I want to scan
    const std::string path = "/Users/matteo/Downloads";
    // convert the string to a path object recognised by C++
    fs::path targetFolder(path);

    // check if the folder exists to avoid crash
    if (!fs::exists(targetFolder))
    {
        std::cout << "Error: The folder does not exist!" << std::endl;
        return 1;
    }

    fs::path imagesFolder = targetFolder / "images";
    fs::path docFolder = targetFolder / "documents";
    fs::path zipFolder = targetFolder / "zipFiles";

    const std::unordered_set<std::string> imagesExtensions = {".png", ".jpg", ".jpeg", ".gif", ".webp", ".bmp"};
    const std::unordered_set<std::string> docExtensions = {".pdf", ".txt", ".doc", ".docx", ".md"};
    const std::unordered_set<std::string> zipExtensions = {".zip", ".7z", ".rar", ".gz", ".bz2"};

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

            if (imagesExtensions.contains(fileExtension.string()))
            {
                // create the directory if it doesn't already exist
                fs::create_directory(imagesFolder);

                // define the new file's path
                fs::path newPath = imagesFolder / fileName;

                // move the file
                fs::rename(filePath, newPath);

                std::cout << "[MOVED] " << fileName << " to images" << std::endl;
            }

            else if (docExtensions.contains(fileExtension.string()))
            {
                fs::create_directory(docFolder);
                fs::path newPath = docFolder / fileName;
                fs::rename(filePath, newPath);

                std::cout << "[MOVED] " << fileName << " to documents" << std::endl;
            }

            else if (zipExtensions.contains(fileExtension.string()))
            {
                fs::create_directory(zipFolder);
                fs::path newPath = zipFolder / fileName;
                fs::rename(filePath, newPath);

                std::cout << "[MOVED] " << fileName << " to zipFiles" << std::endl;
            }
        }
    }


    std::cout << "\n--- DONE ---\n";
    return 0;
}

