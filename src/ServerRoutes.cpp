#include "ServerRoutes.h"
#include "filemanager/fileloader.h"
#include <iostream>

// Setup all web routes
void setupRoutes(httplib::Server &server, FileManager &fileManager) {

  // ===== DISPLAY LOGIN PAGE =====
  server.Get("/login", [](const httplib::Request &, httplib::Response &res) {
    res.set_content(loadFile("web/login.html"), "text/html");
  });

  // ===== DISPLAY REGISTER PAGE =====
  server.Get("/register", [](const httplib::Request &, httplib::Response &res) {
    res.set_content(loadFile("web/register.html"), "text/html");
  });

  // ===== HANDLE USER REGISTRATION =====
  server.Post("/register", [&fileManager](const httplib::Request &req,
                                          httplib::Response &res) {
    // Get username and password from form
    std::string username = req.get_param_value("username");
    std::string password = req.get_param_value("password");

    // Validate input
    if (username.empty() || password.empty()) {
      res.set_content("Username and password cannot be empty", "text/plain");
      return;
    }

    // Try to add user to file
    if (fileManager.addUser(username, password)) {
      res.set_redirect("/login"); // Success! Go to login page
    } else {
      res.set_content("Error creating account - username may already exist",
                      "text/plain");
    }
  });

  // ===== HANDLE USER LOGIN =====
  server.Post("/login", [&fileManager](const httplib::Request &req,
                                       httplib::Response &res) {
    // Get username and password from form
    std::string username = req.get_param_value("username");
    std::string password = req.get_param_value("password");

    // Check if user exists with that username and password
    User *user = fileManager.getUser(username, password);

    if (user != nullptr) {
      // Login successful! Go to home page
      res.set_redirect("/home?username=" + username);
      delete user; // Clean up allocated memory
    } else {
      // Login failed
      res.set_content("Invalid username or password", "text/plain");
    }
  });

  // ===== DISPLAY HOME PAGE =====
  server.Get("/home", [&fileManager](const httplib::Request &req,
                                     httplib::Response &res) {
    // Get username from URL parameter
    std::string username = req.get_param_value("username");

    // If no username, redirect to login
    if (username.empty()) {
      res.set_redirect("/login");
      return;
    }

    // Get user info from file
    User *user = fileManager.getUserByUsername(username);

    if (user != nullptr) {
      // User found! Get their data
      std::string dbUsername = user->username;
      std::string balance = user->balance;

      // Load HTML template
      std::string html = loadFile("web/home.html");

      // Replace placeholders with actual data
      size_t pos = html.find("{{username}}");
      if (pos != std::string::npos) {
        html.replace(pos, 12, dbUsername);
      }

      pos = html.find("{{balance}}");
      if (pos != std::string::npos) {
        html.replace(pos, 11, balance);
      }

      res.set_content(html, "text/html");
      delete user; // Clean up allocated memory
    } else {
      res.set_content("User not found", "text/plain");
    }
  });
}

// Initialize the users file with test data
void initializeFile(FileManager &fileManager) { fileManager.initializeFile(); }
