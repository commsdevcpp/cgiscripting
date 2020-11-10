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

