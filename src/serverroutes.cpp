#include "serverroutes.h"
#include "filemanager/fileloader.h"

#include <iostream>

void setupRoutes(httplib::Server &server, FileManager &fileManager) {

    // Show the home page with user info
    server.Get("/home", [&fileManager](const httplib::Request &req, httplib::Response &res) {
        // Get account ID from URL parameter (e.g., /home?accountId=9304-2318-2384-2384)
        std::string accountId = req.get_param_value("accountId");

        // Get user from file
        User *user = fileManager.getUser(accountId);

        if (!user) {
            res.set_content("User not found!", "text/plain");
            return;
        }

        // Load HTML template
        std::string html = loadFile("web/home.html");

        // Replace placeholders with actual data
        size_t pos = 0;
        while ((pos = html.find("{{username}}", pos)) != std::string::npos) {
            html.replace(pos, 12, user->fullname);
            pos += user->fullname.length();
        }

        pos = 0;
        while ((pos = html.find("{{accountId}}", pos)) != std::string::npos) {
            html.replace(pos, 13, user->accountId);
            pos += user->accountId.length();
        }

        pos = 0;
        while ((pos = html.find("{{balance}}", pos)) != std::string::npos) {
            html.replace(pos, 11, user->balance);
            pos += user->balance.length();
        }

        delete user;
        res.set_content(html, "text/html");
    });

    // Handle deposit, withdraw, transfer
    server.Post("/home", [&fileManager](const httplib::Request &req, httplib::Response &res) {
        std::string accountId = req.get_param_value("accountId");
        std::string action = req.get_param_value("action");

        // Debug logging
        std::cout << "Action: " << action << std::endl;
        std::cout << "Account ID: " << accountId << std::endl;

        if (action == "deposit") {
            std::string amountStr = req.get_param_value("depositAmount");
            std::cout << "Deposit Amount: " << amountStr << std::endl;

            if (amountStr.empty()) {
                res.set_content("No amount entered!", "text/plain");
                return;
            }

            double amount = std::stod(amountStr);
            if (fileManager.deposit(accountId, amount)) {
                res.set_redirect("/home?accountId=" + accountId);
            } else {
                res.set_content("Deposit failed! Check console for errors.", "text/plain");
            }
        } else if (action == "withdraw") {
            double amount = std::stod(req.get_param_value("withdrawAmount"));
            if (fileManager.withdraw(accountId, amount)) {
                res.set_redirect("/home?accountId=" + accountId);
            } else {
                res.set_content("Withdraw failed! Not enough money?", "text/plain");
            }
        } else if (action == "transfer") {
            double amount = std::stod(req.get_param_value("transferAmount"));
            std::string toAccountId = req.get_param_value("toAccountId");
            if (fileManager.transfer(accountId, toAccountId, amount)) {
                res.set_redirect("/home?accountId=" + accountId);
            } else {
                res.set_content("Transfer failed!", "text/plain");
            }
        }
    });
}
