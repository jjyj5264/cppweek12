#ifndef SERVICE_H
#define SERVICE_H

#include "node.h"
#include <algorithm>

class Host;

class Service {
  friend class ServiceInstaller;
  friend class Host;

  protected:
  // 현재 서비스가 설치된 호스트
  Host *host_;

  // 현재 서비스가 사용하는 포트
  short port_;

  // 현재 사용 중인 port의 목록
  static std::vector<short> usedPorts_;

  Service(Host *host, int port) : host_(host), port_(port) {}

  void initialize() {
    if (port_ == -1) {
      port_ = getAvailablePort();
    }
  }

  short getAvailablePort() {
    short port = 1000;

    while (true) {
      bool isUsed = false;

      // find
      for (short usedPort : Service::usedPorts_) {
        if (usedPort == port) {
          isUsed = true;
          break;
        }
      }

      // return if not used
      if (!isUsed) {
        Service::usedPorts_.push_back(port);
        return port;
      }

      // increase if used
      port++;
    }
  }

  virtual void recieve(Packet *packet) = 0;

  public:
  virtual ~Service() {
    // remove port from usedPorts_
    Service::usedPorts_.erase(std::remove(Service::usedPorts_.begin(), Service::usedPorts_.end(), port_), Service::usedPorts_.end());
  }
};

#endif