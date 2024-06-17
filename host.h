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

  public:
  Address address() { return address_; }
  Host(Address address) : address_(address) {}

  std::vector<short> &ports() { return ports_; }

  // 호스트와 설치된 서비스를 전부 초기화한다.
  void initialize() {
    for (Service *service : services_) {
      service->initialize();
    }

    // std::cout << "Host #" << id() << ": ports: ";
    // for (short port : ports_) {
    //   std::cout << port << " ";
    // }
    // std::cout << std::endl;
  }

  // 링크를 랜덤으로 하나 선택하여 패킷을 전송한다.
  void send(Packet *packet) {
    int linkIndex = rand() % links_.size();
    Link *selectedLink = links_[linkIndex];

    std::cout << "Host #" << id() << ": sending packet (from: " << packet->srcAddress().toString()
              << ", to: " << packet->destAddress().toString() << ", " << packet->size() << " bytes)" << std::endl;
    selectedLink->forwardPacket(packet, this);
  }

  // virtual destructor
  virtual ~Host() {
    for (Service *service : services_) {
      delete service;
    }
  }

  // push back service to services_
  void addService(Service *service) {
    services_.push_back(service);
  }

  void receive(Packet *packet) {
    // std::cout << "Installed Services(" << id() << "): " << std::endl;
    // for (Service *service : services_) {
    //   std::cout << service->port_ << " ";
    // }
    // std::cout << std::endl;
    // 여러 service가 receive를 호출할 수 없는 이유는 host 내에서 port가 전부 다르기 때문이다.
    // 그러므로 일치하는 케이스에서는 return;으로 종료해야 프로그램이 정상적으로 작동한다.
    for (Service *service : services_) {
      if (packet->destAddress() == address_ && packet->destPort() == service->port_) {
        std::cout << "Host #" << id() << ": received packet, destination port: " << packet->destPort() << std::endl;
        service->recieve(packet); // send packet to service
        return;
      }
    }

    // if packet does not match any service, delete it
    std::cout << "Host #" << id() << ": no service for packet (from: " << packet->srcAddress().toString()
              << ", to: " << packet->destAddress().toString() << ", " << packet->size() << " bytes)" << std::endl;
    delete packet;
  }
};

#endif