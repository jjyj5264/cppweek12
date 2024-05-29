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

  // 설치된 서비스들이 사용하는 포트 목록
  std::vector<int> ports_;

  public:
  Address address() { return address_; }
  Host(Address address) : address_(address) {}

  // 포트 목록에 포트를 추가한다.
  void addPort(int port) {
    ports_.push_back(port);
  }

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