#ifndef SERVICE_INSTALLER_H
#define SERVICE_INSTALLER_H

#include "host.h"
#include "service.h"

// 서비스를 설치하는 역할
class ServiceInstaller {
  protected:
  // 호스트와 서비스 사이의 관계를 설정한다.
  void install(Host *host, Service *service) {
    host->addService(service);
    service->host_ = host; // Service에 Service가 설치된 Host의 포인터 주소를 저장한다.
  }

  void addUsedPort(short port) {
    Service::usedPorts_.push_back(port);
  }
};

#endif