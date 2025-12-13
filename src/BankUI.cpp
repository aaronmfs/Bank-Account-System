#include "include/BankUI.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

BankUI::BankUI(int width, int height)
    : m_webview(nullptr), m_width(width), m_height(height),
      m_initialized(false) {}

BankUI::~BankUI() {
  if (m_webview) {
    delete m_webview;
  }
}

void BankUI::initialize() {
  if (m_initialized) {
    return;
  }

  try {
    m_webview = new webview::webview(false, nullptr);
    m_webview->set_title("Bank Account System");
    m_webview->set_size(m_width, m_height, WEBVIEW_HINT_NONE);
    m_initialized = true;
  } catch (const webview::exception &e) {
    throw std::runtime_error(std::string("Failed to initialize UI: ") +
                             e.what());
  }
}

void BankUI::run() {
  if (!m_initialized || !m_webview) {
    throw std::runtime_error("UI not initialized. Call initialize() first.");
  }

  m_webview->run();
}

void BankUI::setHTML(const std::string &html) {
  if (!m_initialized || !m_webview) {
    throw std::runtime_error("UI not initialized");
  }

  m_webview->set_html(html);
}

void BankUI::loadHTMLFile(const std::string &filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open HTML file: " + filepath);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  setHTML(buffer.str());
}

void BankUI::bindFunction(
    const std::string &name,
    std::function<std::string(const std::string &)> callback) {
  if (!m_initialized || !m_webview) {
    throw std::runtime_error("UI not initialized");
  }

  m_webview->bind(name, callback);
}

void BankUI::navigate(const std::string &url) {
  if (!m_initialized || !m_webview) {
    throw std::runtime_error("UI not initialized");
  }

  m_webview->navigate(url);
}

void BankUI::eval(const std::string &js) {
  if (!m_initialized || !m_webview) {
    throw std::runtime_error("UI not initialized");
  }

  m_webview->eval(js);
}

void BankUI::terminate() {
  if (m_initialized && m_webview) {
    m_webview->terminate();
  }
}
