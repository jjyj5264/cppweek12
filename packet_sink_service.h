#ifndef PACKET_SINK_SERVICE_H
#define PACKET_SINK_SERVICE_H

#include "service.h"
#include <string>

class PacketSinkService : public Service {
  friend class PacketSinkServiceInstaller;

  private:
  int totalPacketSize = 0;

  PacketSinkService(Host *host, short port) : Service(host, port) {} // port는 scenario에서 알아서 줌
  std::string name() override { return "PacketSinkService"; }

  public:
  void receive(Packet *packet) override {
    totalPacketSize += packet->data().size();
    log("received total " + std::to_string(totalPacketSize) + " bytes");
    delete packet;
  }

  // ~PacketSinkService() { log("~PacketSinkService() called"); }
};

#endif