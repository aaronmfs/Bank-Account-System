#include "FileManager.h"
#include "ServerRoutes.h"
#include <iostream>

int main() {
  // Create file manager (will use users.txt by default)
  FileManager fileManager("users.txt");

  // Initialize file with test data if it doesn't exist
  initializeFile(fileManager);

  // Create HTTP server
  httplib::Server server;

  // Setup all routes (login, register, home pages)
  setupRoutes(server, fileManager);

  // Start the server
  std::cout << "Server running on http://localhost:8080\n";
  std::cout << "Test user - Username: aaronmathew, Password: password\n";
  server.listen("0.0.0.0", 8080);

  return 0;
}
