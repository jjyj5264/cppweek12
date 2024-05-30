#include "service.h"
#include "host.h"

void Service::initialize() {
  // std::cout << "Service::initialize()" << std::endl;
  if (port_ == -1) {
    // std::cout << "Service::initialize() - port_ is -1" << std::endl;
    port_ = getAvailablePort();
  } else { // for manually assigned port numbers like ECHO_PORT
    // std::cout << "Service::initialize() - port_ is not -1" << std::endl;
    host_->ports().push_back(port_);
  }
}

short Service::getAvailablePort() {
  // std::cout << "Service::getAvailablePort()" << std::endl;
  short port = 1000;
  while (std::find(host_->ports().begin(), host_->ports().end(), port) != host_->ports().end()) {
    port++;
  }
  host_->ports().push_back(port);
  return port;
}
