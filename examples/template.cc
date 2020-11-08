#include <ctdlib.h>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv) {
  srand(time(NULL));

  cgi::script s;
  cgi::template_content tmpl_data;

  s.set_content_type("text/html");

  tmpl_data.set_pair("foo", "bar");
  tmpl_data.set_pair("bar", 12345);
  tmpl_data.set_pair("baz", rand());
  
  s.send_template("../templates/my.template.html", tmpl_data);
  return 0;
}

