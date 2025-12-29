#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <string>

// Define where our HTML files are stored
#ifdef RESOURCES_PATH
#define RESOURCE_PATH RESOURCES_PATH
#else
#define RESOURCE_PATH "./assets/"
#endif

// This function loads a file and returns its contents as a string
std::string loadFile(const std::string &filename);

#endif
