#include "Bank.h"
#include "httplib/httplib.h"
#include <fstream>
#include <iostream>
#include <sstream>

// Helper function to read file content
std::string readFile(const std::string &filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    Bank bank;
    httplib::Server svr;

    // Serve static files (HTML, CSS, JS)
    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        std::string content = readFile(RESOURCES_PATH "html/index.html");
        if (content.empty()) {
            res.status = 404;
            res.set_content("index.html not found", "text/plain");
        } else {
            res.set_content(content, "text/html");
        }
    });

    svr.Get("/styles.css", [](const httplib::Request &, httplib::Response &res) {
        std::string content = readFile(RESOURCES_PATH "css/styles.css");
        res.set_content(content, "text/css");
    });

    svr.Get("/script.js", [](const httplib::Request &, httplib::Response &res) {
        std::string content = readFile(RESOURCES_PATH "js/script.js");
        res.set_content(content, "application/javascript");
    });

    // REST API Endpoints - return JSON
    svr.Post("/api/create", [&bank](const httplib::Request &req, httplib::Response &res) {
        std::string name = req.get_param_value("name");
        double amount = std::stod(req.get_param_value("amount"));
        std::string pin = req.get_param_value("pin");

        std::string json = bank.createAccountAPI(name, amount, pin);
        res.set_content(json, "application/json");
    });

    svr.Post("/api/deposit", [&bank](const httplib::Request &req, httplib::Response &res) {
        int account = std::stoi(req.get_param_value("account"));
        std::string pin = req.get_param_value("pin");
        double amount = std::stod(req.get_param_value("amount"));

        std::string json = bank.depositAPI(account, pin, amount);
        res.set_content(json, "application/json");
    });

    svr.Post("/api/withdraw", [&bank](const httplib::Request &req, httplib::Response &res) {
        int account = std::stoi(req.get_param_value("account"));
        std::string pin = req.get_param_value("pin");
        double amount = std::stod(req.get_param_value("amount"));

        std::string json = bank.withdrawAPI(account, pin, amount);
        res.set_content(json, "application/json");
    });

    svr.Post("/api/balance", [&bank](const httplib::Request &req, httplib::Response &res) {
        int account = std::stoi(req.get_param_value("account"));
        std::string pin = req.get_param_value("pin");

        std::string json = bank.checkBalanceAPI(account, pin);
        res.set_content(json, "application/json");
    });

    svr.Post("/api/transfer", [&bank](const httplib::Request &req, httplib::Response &res) {
        int fromAccount = std::stoi(req.get_param_value("from_account"));
        std::string pin = req.get_param_value("pin");
        int toAccount = std::stoi(req.get_param_value("to_account"));
        double amount = std::stod(req.get_param_value("amount"));

        std::string json = bank.transferAPI(fromAccount, pin, toAccount, amount);
        res.set_content(json, "application/json");
    });

    std::cout << "Server starting on http://localhost:8080" << std::endl;
    std::cout << "Serving files from 'assets' folder" << std::endl;
    svr.listen("0.0.0.0", 8080);

    return 0;
}
