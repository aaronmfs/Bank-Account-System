#include <filesystem>
#include <string>
#include <webview/webview.h>

int main() {
  webview_t w = webview_create(0, nullptr);
  webview_set_title(w, "Bank Account System");
  webview_set_size(w, 800, 600, WEBVIEW_HINT_NONE);

  std::string base = std::filesystem::current_path().string();
  std::string url = "file:///" + base + "/assets/html/index.html";

  webview_navigate(w, url.c_str());

  webview_run(w);
  webview_destroy(w);
  return 0;
}
