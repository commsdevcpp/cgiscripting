#include <cstdlib.h>

int main(int argc, char** argv) {
  cgi::script s;
  s.use_mimedetect();
  
  // Inherently overrides user content type by detecting this file type
  // (JSON) and sending Content-Type: application/json
  cgi::send_static("../static/file.json");
  return 0;
}

