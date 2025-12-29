#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>

// Structure to hold user data
struct User {
  std::string username;
  std::string password;
  std::string balance;
};

class FileManager {
private:
  std::string filename;

  // Helper function to split a string by delimiter
  std::vector<std::string> split(const std::string &str, char delimiter);

  // Helper function to trim whitespace
  std::string trim(const std::string &str);

public:
  // Constructor
  FileManager(const std::string &file = "users.txt");

  // User operations
  bool addUser(const std::string &username, const std::string &password);
  User *getUser(const std::string &username, const std::string &password);
  User *getUserByUsername(const std::string &username);
  bool updateBalance(const std::string &username,
                     const std::string &newBalance);

  // Initialize file with test data
  void initializeFile();

  // Check if user exists
  bool userExists(const std::string &username);
};

#endif // FILE_MANAGER_H
