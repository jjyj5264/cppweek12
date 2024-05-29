#ifndef MESSAGE_SERVICE_H
#define MESSAGE_SERVICE_H

#include "address.h"
#include "host.h"
#include "service.h"
#include <iostream>

// send 함수를 호출하여 메시지를 전송할 수 있는 서비스
class MessageService : public Service {
  friend class MessageServiceInstaller;

  private:
  // 목적지 주소
  Address destAddress_;
  // 목적지 포트
  short destPort_;

  // !!! port는 이 서비스의 포트 !!!
  MessageService(Host *host, short port, Address destAddress, short destPort)
      : Service(host, port), destAddress_(destAddress), destPort_(destPort) {}

  public:
  // 메시지를 전송한다
  void send(std::string message) {
    // srcAddress_: 해당 서비스가 설치된 호스트의 주소
    // srcPort_: '서비스의' 포트
    Address srcAddress_ = host_->address();
    short srcPort_ = port_;

    // this should be deleted in proper time
    Packet *packet = new Packet(srcAddress_, destAddress_, srcPort_, destPort_, message);

    // 패킷을 호스트에 전달
    host_->send(packet);
  }

  void recieve(Packet *packet) {
    std::cout << "MessageService: received \"" << packet->dataString()
    << "\" from " << packet->srcAddress().toString() << ":" << packet->srcPort() << std::endl;

    delete packet;
  }
};

#endif