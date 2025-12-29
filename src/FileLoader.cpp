#include "FileLoader.h"
#include <fstream>
#include <string>

// Load a file and return its contents as a string
std::string loadFile(const std::string &filename) {
  // Build full path (e.g., "./assets/web/login.html")
  std::string fullPath = RESOURCE_PATH + filename;

  // Open the file
  std::ifstream file(fullPath);

  // Read entire file into a string
  std::string content((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());

  return content;
}
