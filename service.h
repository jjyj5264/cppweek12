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

  Service(Host *host, int port) : host_(host), port_(port) {}

  void initialize();
  short getAvailablePort();
  virtual void recieve(Packet *packet) = 0;

  public:
  virtual ~Service() {}
};

#endif