#ifndef PING_SERVICE_INSTALLER_H
#define PING_SERVICE_INSTALLER_H

#include "address.h"
#include "message_service.h"
#include "service_installer.h"

class MessageServiceInstaller : public ServiceInstaller {
  private:
  // 목적지 주소
  Address destAddress_;
  // 목적지 포트
  short destPort_;

  public:
  MessageServiceInstaller(const Address &destAddress, short destPort)
      : destAddress_(destAddress), destPort_(destPort) {}

  // 호스트에 MessageService를 설치한다
  MessageService *install(Host *host) {
    // 포트는 service 클래스에서 지정할 것이므로, 따로 지정하지 않는다.
    MessageService *messageService = new MessageService(host, -1, destAddress_, destPort_);
    ServiceInstaller::install(host, messageService);
    
    return messageService;
  }
};

#endif