#include "service.h"
#include "host.h"

void Service::initialize() {
  if (port_ == -1) {
    port_ = getAvailablePort();
  } else { // for manually assigned port numbers like ECHO_PORT
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
