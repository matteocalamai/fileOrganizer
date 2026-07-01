#include <iostream>
#include <filesystem>

#define extCheck element.path().extension()

using namespace std;
namespace fs = filesystem;

int main()
{

  // define the path of the folder that I want to scan
  const string path = "/Users/matteo/Downloads";
  // convert the string to a path object recognised by C++
  fs::path targetFolder(path);

  // check if the folder exists to avoid crash
  if (!fs::exists(targetFolder))
  {
    cout << "Error: The folder does not exist!" << endl;
    return 1;
  }

  fs::path imagesFolder = targetFolder / "images";
  fs::path docFolder = targetFolder / "documents";
  fs::path zipFolder = targetFolder / "zipFiles";

  cout << "--- Folder: " << targetFolder << " ---" << endl;

  // go through the whole directory and print every file that's in it
  for (const auto& element : fs::directory_iterator(targetFolder))
  {

    if (fs::is_regular_file(element))
    {
      cout << "[FILE] " << element.path().filename().string() << endl;

      fs::path fileExtension = element.path().extension();
      fs::path filePath = element.path();
      fs::path fileName = element.path().filename();

      if (extCheck == ".png" || extCheck == ".jpg" || extCheck == ".gif" || extCheck == ".jpeg")
      {
        // create the directory if it doesn't already exist
        fs::create_directory(imagesFolder);

        // define the new file's path
        fs::path newPath = imagesFolder / fileName;

        // move the file
        fs::rename(filePath, newPath);

        cout << "[MOVED] " << fileName << " to images" << endl;
      }

      if (extCheck == ".txt" || extCheck == ".pdf" || extCheck == ".md")
      {
        fs::create_directory(docFolder);
        fs::path newPath = docFolder / fileName;
        fs::rename(filePath, newPath);

        cout << "[MOVED] " << fileName << " to documents" << endl;
      }

      if (extCheck == ".zip" || extCheck == ".gz" || extCheck == ".bz2" || extCheck == ".7z" || extCheck == ".rar")
      {
        fs::create_directory(zipFolder);
        fs::path newPath = zipFolder / fileName;
        fs::rename(filePath, newPath);

        cout << "[MOVED] " << fileName << " to zipFiles" << endl;
      }

    }

  }


  cout << "\n--- DONE ---\n";
  return 0;
}

