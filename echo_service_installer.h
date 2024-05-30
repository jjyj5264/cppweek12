#ifndef ECHO_SERVICE_INSTALLER_H
#define ECHO_SERVICE_INSTALLER_H

#include "address.h"
#include "echo_service.h"
#include "service_installer.h"

class EchoServiceInstaller : public ServiceInstaller {
private:
  // 패킷을 수신할 포트
  short listenPort_; // in first.cpp: 3000

public:
  EchoServiceInstaller(short listenPort) : listenPort_(listenPort) {}

  // 호스트에 EchoService를 설치한다.
  EchoService *install(Host *host) {
    EchoService *service = new EchoService(host, listenPort_);
    ServiceInstaller::install(host, service);
    host->ports().push_back(listenPort_); // do this only for EchoService

    return service;
  }
};

#endif