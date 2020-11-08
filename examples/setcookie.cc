#include <cgilib.h>

int main(int argc, char** argv) {
  cgi::script s;

  s.set_content_type("text/html");
  
  s.set_cookie("mykey", "myvalue");
  s.set_cookie("second_key", "ahugetokenthatnobodycaresabout");
  
  if(s.has_reqcookie("x")) {
    if(s.get_reqcookie("x") == "ping") {
      s.set_cookie("x", "pong");
    } else {
      s.set_cookie("x", "ping");
    }
  }

  s.out << "I've set some cookies for you!";
  
  s.send_response();
  return 0;
}

