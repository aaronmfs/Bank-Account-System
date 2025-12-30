#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>

struct User {
    std::string accountId;
    std::string email;
    std::string password;
    std::string fullname;
    std::string balance;
};

class FileManager {
  private:
    std::string filename;
    bool updateBalance(const std::string &accountId, const std::string &newBalance);

  public:
    FileManager(const std::string &file);

    User *getUser(const std::string &accountId);
    bool deposit(const std::string &accountId, double amount);
    bool withdraw(const std::string &accountId, double amount);
    bool transfer(const std::string &from, const std::string &to, double amount);
};

#endif
