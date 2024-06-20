#ifndef HOST_H
#define HOST_H

#include "address.h"
#include "link.h"
#include "packet.h"
#include "service.h"
#include <iostream>
#include <vector>

class Host : public Node {
  friend class ServiceInstaller;

  private:
  // 호스트의 주소
  Address address_;

  // 설치된 서비스 목록
  std::vector<Service *> services_;

  // 현재 호스트에 설치된 모든 서비스들의 포트 번호 목록
  std::vector<short> ports_;

  std::string name() override { return "Host"; }

  public:
  Address address() { return address_; }
  Host(Address address) : address_(address) {}

  std::vector<short> &ports() { return ports_; }

  // 호스트와 설치된 서비스를 전부 초기화한다.
  void initialize() {
    for (Service *service : services_) {
      service->initialize();
    }
  }

  // 링크를 랜덤으로 하나 선택하여 패킷을 전송한다.
  void send(Packet *packet) {
    int linkIndex = rand() % links_.size();
    Link *selectedLink = links_[linkIndex];
    std::string packetID = packet->toString();

    log("sending packet: " + packetID);
    selectedLink->forwardPacket(packet, this);
  }

  ~Host() {
    // log("list of services installed in this host: ");
    // for (size_t i = 0; i < services_.size(); i++) {
    //   log(services_[i]->toString());
    // }
    // log("~Host() called. Deleting services...");
    for (size_t i = 0; i < services_.size(); i++) {
      // log("deleting service: " + services_[i]->toString());
      delete services_[i];
    }
    services_.clear();
  }

  void addService(Service *service) {
    services_.push_back(service);
  }

  void receive(Packet *packet) {
    std::string destAddress = packet->destAddress().toString(); // to
    std::string packetID = packet->toString();

    // 여러 service가 receive를 호출할 수 없는 이유는 host 내에서 port가 전부 다르기 때문이다.
    // 그러므로 일치하는 케이스에서는 return;으로 종료해야 프로그램이 정상적으로 작동한다.
    for (Service *service : services_) {
      if (packet->destAddress() == address_ && packet->destPort() == service->port_) {
        log("received packet: " + packetID + ", forwarding to " + service->toString());
        service->receive(packet); // send packet to service
        return;
      }
    }

    // if packet does not match any service, delete it
    log("no service for packet: " + packetID);
    delete packet;
  }
};

#endif