#ifndef BULK_SEND_SERVICE_H
#define BULK_SEND_SERVICE_H

#include "host.h"
#include "service.h"
#include "simulator.h"

#define PACKET_SIZE 512

class BulkSendService : Service {
  friend class BulkSendServiceInstaller;

  private:
  BulkSendService(Host *host, Address destAddress, short destPort,
                  double delay = 1, double startTime = 0,
                  double stopTime = 10.0)
                  : Service(host, -1) {}; // port_는 initialize() 단계에서 설정될 것임

  std::string name() override { return "BulkSendService"; }

  public:
  // ~BulkSendService() { log("~BulkSendService() called"); }

  void bulkSend(Host *host, Address destAddress, short destPort) {
    std::string data(PACKET_SIZE, 'A');

    log("sending data");
    // 자신의 port_는 initialize() 단계에서 설정될 것임
    Packet *packet = new Packet(host->address(), destAddress, port_, destPort, data);
    host->send(packet); // 자신이 설치된 host가 packet을 보냄
  }

  void receive(Packet *packet) override {
    delete packet; // 딱히 log를 출력하지는 않는 듯
  }
};

#endif