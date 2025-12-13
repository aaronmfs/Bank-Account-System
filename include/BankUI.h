#ifndef BANK_UI_H
#define BANK_UI_H

#include "webview/webview.h"
#include <functional>
#include <string>

class BankUI {
public:
  BankUI(int width = 800, int height = 600);
  ~BankUI();

  // Initialize and run the UI
  void initialize();
  void run();

  // Set HTML content
  void setHTML(const std::string &html);
  void loadHTMLFile(const std::string &filepath);

  // Bind C++ functions to JavaScript
  void bindFunction(const std::string &name,
                    std::function<std::string(const std::string &)> callback);

  // Navigate
  void navigate(const std::string &url);

  // Execute JavaScript
  void eval(const std::string &js);

  // Terminate the UI
  void terminate();

private:
  webview::webview *m_webview;
  int m_width;
  int m_height;
  bool m_initialized;
};

#endif // BANK_UI_H
