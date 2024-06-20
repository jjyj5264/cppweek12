#ifndef SERVICE_H
#define SERVICE_H

#include "node.h"
#include "object.h"
#include <algorithm>

class Host;

class Service : public Object {
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
  virtual void receive(Packet *packet) = 0;

  public:
  std::string toString() { return Object::toString(); }
};

#endif