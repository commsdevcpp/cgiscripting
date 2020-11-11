#ifndef COMMSDEVCPP_CGILIB_H_
#define COMMSDEVCPP_CGILIB_H_

#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream>

namespace cgi {

//// DATA STRUCTURES ////
typedef struct singluar_header {
  std::string value = "";
  bool exists = false;
} singular_header_t;

//// SCRIPT CLASS ////

class script {
public:
  script() {
    
  }

  //// METAVARIABLES ////
  std::string get_auth_type() {
    const char* envvar = std::getenv("AUTH_TYPE");

    if(envvar == nullptr) { return ""; }
    return envvar;
  }

  int get_request_content_length() {
    const char* envvar = std::getenv("CONTENT_LENGTH");

    if(envvar == nullptr || envvar == "") return 0;
    return std::atoi(envvar);
  }

  std::string get_request_content_type() {
    const char* envvar = std::getenv("CONTENT_TYPE");

    if(envvar == nullptr) return "";
    return envvar;
  }

  std::string gateway_interface() {
    return std::getenv("GATEWAY_INTERFACE");
  }

  std::string get_subresource_uri() {
    const char* envvar = std::getenv("PATH_INFO")

    if(envvar == nullptr) return "";
    return envvar;
  }

  std::string get_subresource_wouldbe_path() {
    const char* envvar = std::getenv("PATH_TRANSLATED");

    if(envvar == nullptr) return "";
    return envvar;
  }

  std::string get_query_string() {
    return std::getenv("QUERY_STRING");
  }

  std::string get_client_address() {
    return std::getenv("REMOTE_ADDR");
  }

  std::string get_client_hostname() {
    const char* envvar = std::getenv("REMOTE_HOST");

    if(envvar == nullptr) return std::getenv("REMOTE_ADDR");
    return envvar;
  }

  std::string get_connection_identity() {
    const char* envvar = std::getenv("REMOTE_IDENT");

    if(envvar == nullptr) return "";
    return envvar;
  }

  std::string get_client_identity() {
    const char* envvar = std::getenv("REMOTE_USER");

    if(envvar == nullptr) return "";
    return envvar;
  }

  std::string get_request_method() {
    return std::getenv("REQUEST_METHOD");
  }

  std::string get_script_name() {
    return std::getenv("SCRIPT_NAME");
  }

  std::string get_server_name() {
    return std::getenv("SERVER_NAME");
  }

  int get_server_port() {
    return std::atoi(std::getenv("SERVER_PORT"));
  }

  std::string get_server_protocol() {
    return std::getenv("SERVER_PROTOCOL");
  }

  std::string get_server_software() {
    return std::getenv("SERVER_SOFTWARE");
  }

  //// RESPONSE HEADERS ////

  // Content-Type
  singular_header_t reshed_content_type_;
  void content_type(std::string mime) {
    reshed_content_type_.value = mime;
    reshed_content_type_.exists = true;
  }

  void clear_content_type() {
    reshed_content_type_.exists = false;
  }

  // Access-Control-Allow-Origin
  singular_header_t reshed_access_control_allow_origin_;
  void global_resource() {
    const char* origin_header = std::getenv("HTTP_ORIGIN");
    if(origin_header == nullptr) {
      reshed_access_control_allow_origin_.value = "*";
    } else {
      reshed_access_control_allow_origin_.value = origin_header;
    }
    
    reshed_access_control_allow_origin_.exists = true;
  }

  void allow_origin(std::string origin) {
    reshed_access_control_allow_origin_.value = origin;
    reshed_access_control_allow_origin_.exists = true;
  }

  void disallow_origin(std::string origin) {
    const char* origin_header = std::getenv("HTTP_ORIGIN");
    if(origin_header == nullptr) {
      reshed_access_control_allow_origin_.value = "null";
    } else if(origin == origin_header) {
      reshed_access_control_allow_origin_.value = "null";
    } else {
      reshed_access_control_allow_origin_.value = origin_header;
    }

    reshed_access_control_allow_origin_.exists = true;
  }

  void allow_origins(std::vector<std::string> origins) {
    const char* origin_header = std::getenv("HTTP_ORIGIN");
    if(origin_header == nullptr) {
      // Clearly the user wants to restrict access, and credentials weren't given.
      // Deny access
      reshed_access_control_allow_origin_.value = "null";  // TEMPORARY; does not actually get sent.
    }
    
    bool found = false;
    for(int i = 0; i < origins.size(); ++i) {
      if(origins[i] == origin_header) {
        reshed_access_control_allow_origin_.value = origins[i];
        found = true;

        break;
      }
    }

    if(!found) {
      // The request is explicitly coming from an origin not on the allow list.
      reshed_access_control_allow_origin_.value = "null";
    }

    reshed_access_control_allow_origin_.exists = true;
  }

  void disallow_origins(std::vector<std::string> origins) {
    const char* origin_header = std::getenv("HTTP_ORIGIN");
    if(origin_header == nullptr) {
      // Clearly the user wants to restrict access, and credentials weren't given.
      // Deny access
      reshed_access_control_allow_origin_.value = "null";  // TEMPORARY; does not actually get sent.
    }
    
    bool found = false;
    for(int i = 0; i < origins.size(); ++i) {
      if(origins[i] == origin_header) {
        reshed_access_control_allow_origin_.value = "null";
        found = true;

        break;
      }
    }

    if(!found) {
      // The request is not coming from an origin on the disallow list.
      reshed_access_control_allow_origin_.value = origin_header;
    }

    reshed_access_control_allow_origin_.exists = true;
  }

  void allow_anonymous_origin() {
    const char* origin_header = std::getenv("HTTP_ORIGIN");
    if(origin_header == nullptr) {
      reshed_access_control_allow_origin_.value = "*";
      reshed_access_control_allow_origin_.exists = true;
    }
  }

  void disallow_anonymous_origin() {
    const char* origin_header = std::getenv("HTTP_ORIGIN");
    if(origin_header == nullptr) {
      reshed_access_control_allow_origin_.value = "null";
      reshed_access_control_allow_origin_.exists = true;
    }
  }

  void clear_origin_restrictions() {
    reshed_access_control_allow_origin_.exists = false;
  }

  //// CONTENT SENDING ////
  std::ostringstream out;
  void print(std::string v) {
    out << v;
  }

  //// RESPONSE SENDING ////
  void send_response() {
    // Content-Type
    if(reshed_content_type_.exists) {
      std::cout << "Content-Type: " << reshed_content_type_.value << std::endl;
    } else {
      // This header is required as part of the CGI spec
      std::cout << "Content-Type: application/octet-stream" << std::endl;
    }

    // Access-Control-Allow-Origin
    if(reshed_access_control_allow_origin_.exists) {
      if(reshed_access_control_allow_origin_.value != "null") {
        // To be safe, don't send a header if we're unsure,
        // because clients will treat it such that a different origin
        // from the request cannot access the content.
        std::cout << "Access-Control-Allow-Origin: " << reshed_access_control_allow_origin_.value << std::endl;
      }
    }

    //// END HEADER FIELDS ////
    std::cout << std::endl;

    //// ENTITY BODY ////
    std::cout << out.str() << std::endl;
  }
};

} // namespace cgi

#endif  // COMMSDEVCPP_CGILIB_H_

