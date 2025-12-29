#include "FileManager.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

FileManager::FileManager(const std::string &file) : filename(file) {}

// Split string by delimiter
std::vector<std::string> FileManager::split(const std::string &str,
                                            char delimiter) {
  std::vector<std::string> tokens;
  std::stringstream ss(str);
  std::string token;

  while (std::getline(ss, token, delimiter)) {
    tokens.push_back(trim(token));
  }

  return tokens;
}

// Trim whitespace from string
std::string FileManager::trim(const std::string &str) {
  size_t first = str.find_first_not_of(" \t\n\r");
  if (first == std::string::npos)
    return "";

  size_t last = str.find_last_not_of(" \t\n\r");
  return str.substr(first, (last - first + 1));
}

// Add a new user to the file
bool FileManager::addUser(const std::string &username,
                          const std::string &password) {
  // Check if user already exists
  if (userExists(username)) {
    std::cerr << "User already exists\n";
    return false;
  }

  // Open file in append mode
  std::ofstream file(filename, std::ios::app);

  if (!file.is_open()) {
    std::cerr << "Failed to open file for writing\n";
    return false;
  }

  // Write user data: username,password,balance
  file << username << "," << password << ",0\n";
  file.close();

  std::cout << "User added successfully\n";
  return true;
}

// Get user by username and password (for login)
User *FileManager::getUser(const std::string &username,
                           const std::string &password) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    std::cerr << "Failed to open file for reading\n";
    return nullptr;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::vector<std::string> parts = split(line, ',');

    if (parts.size() >= 3) {
      if (parts[0] == username && parts[1] == password) {
        User *user = new User();
        user->username = parts[0];
        user->password = parts[1];
        user->balance = parts[2];
        file.close();
        return user;
      }
    }
  }

  file.close();
  return nullptr;
}

// Get user by username only
User *FileManager::getUserByUsername(const std::string &username) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    std::cerr << "Failed to open file for reading\n";
    return nullptr;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::vector<std::string> parts = split(line, ',');

    if (parts.size() >= 3) {
      if (parts[0] == username) {
        User *user = new User();
        user->username = parts[0];
        user->password = parts[1];
        user->balance = parts[2];
        file.close();
        return user;
      }
    }
  }

  file.close();
  return nullptr;
}

// Update user's balance
bool FileManager::updateBalance(const std::string &username,
                                const std::string &newBalance) {
  std::ifstream fileIn(filename);

  if (!fileIn.is_open()) {
    std::cerr << "Failed to open file for reading\n";
    return false;
  }

  // Read all users into memory
  std::vector<std::string> lines;
  std::string line;
  bool found = false;

  while (std::getline(fileIn, line)) {
    std::vector<std::string> parts = split(line, ',');

    if (parts.size() >= 3 && parts[0] == username) {
      // Update this user's balance
      lines.push_back(parts[0] + "," + parts[1] + "," + newBalance);
      found = true;
    } else {
      lines.push_back(line);
    }
  }

  fileIn.close();

  if (!found) {
    std::cerr << "User not found\n";
    return false;
  }

  // Write all data back to file
  std::ofstream fileOut(filename);

  if (!fileOut.is_open()) {
    std::cerr << "Failed to open file for writing\n";
    return false;
  }

  for (const auto &l : lines) {
    fileOut << l << "\n";
  }

  fileOut.close();
  std::cout << "Balance updated successfully\n";
  return true;
}

// Check if user exists
bool FileManager::userExists(const std::string &username) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    return false;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::vector<std::string> parts = split(line, ',');

    if (parts.size() >= 1 && parts[0] == username) {
      file.close();
      return true;
    }
  }

  file.close();
  return false;
}

// Initialize file with test data
void FileManager::initializeFile() {
  // Check if file already exists
  std::ifstream checkFile(filename);
  if (checkFile.is_open()) {
    checkFile.close();
    std::cout << "Users file already exists\n";
    return;
  }

  // Create new file with test user
  std::ofstream file(filename);

  if (!file.is_open()) {
    std::cerr << "Failed to create users file\n";
    return;
  }

  // Add test user: username,password,balance
  file << "aaronmathew,password,100000\n";
  file.close();

  std::cout << "Users file created with test user\n";
}
