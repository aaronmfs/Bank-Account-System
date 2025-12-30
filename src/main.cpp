#include "filemanager/filemanager.h"
#include "serverroutes.h"

int main() {
    FileManager fileManager("users.txt");
    httplib::Server server;

    setupRoutes(server, fileManager);

    server.listen("0.0.0.0", 8080);
    return 0;
}
