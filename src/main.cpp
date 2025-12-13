#include "include/BankApp.h"
#include <exception>
#include <iostream>

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#else
int main() {
#endif
  try {
    // Create the application
    BankApp app;

    // Initialize all components
    app.initialize();

    // Run the main loop (blocking)
    app.run();

    // Cleanup on exit
    app.shutdown();

    return 0;

  } catch (const std::exception &e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return 1;
  }
}
