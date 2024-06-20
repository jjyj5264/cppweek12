  #ifndef ECHO_SERVICE_H
#define ECHO_SERVICE_H

#include "host.h"
#include "service.h"
#include <iostream>
#include <string>

// 수신한 패킷을 전송자에게 다시 전송하는 서비스
class EchoService : public Service {
  friend class EchoServiceInstaller;

  private:
  EchoService(Host *host, short port) : Service(host, port) {}

  std::string name() override { return "EchoService"; }

  public:
  void receive(Packet *packet) override {
    std::string fromAddress = packet->srcAddress().toString();
    std::string fromPort = std::to_string(packet->srcPort());
    std::string data = packet->dataString();
    log("received \"" + data + "\" from " + fromAddress + ":" + fromPort + ", send reply with same data");
    
    Address srcAddress_ = host_->address();
    short srcPort_ = port_;

    // this should be deleted in proper time
    Packet *newPacket = new Packet(srcAddress_, packet->srcAddress(), srcPort_, packet->srcPort(), packet->dataString());

    // 패킷을 호스트에 전달
    host_->send(newPacket);

    delete packet;
  }
};

#endif