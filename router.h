#ifndef ROUTER_H
#define ROUTER_H

#include "node.h"
#include <iostream>

struct RoutingEntry {
  public:
  Address destination;
  Link *nextLink;
};

class Router : public Node {
  protected:
  std::vector<RoutingEntry> routingTable_;

  public:
  // 패킷을 받으면 라우팅 테이블에 따라 패킷을 다음 링크로 전송한다.
  // 같은 Address를 가진 링크가 여러 개일 경우, 먼저 추가된 링크가 우선순위가 높다.
  void receive(Packet *packet) {
    // 1. check destination address
    Address destAddress = packet->destAddress();

    // 2. find the next link(the first one) to send packet
    bool found = false;

    for (RoutingEntry &entry : routingTable_) { // & isn't necessary
      if (entry.destination == destAddress) {
        std::cout << "Router #" << id() << ": forwarding packet (from: " << packet->srcAddress().toString()
                  << ", to: " << packet->destAddress().toString() << packet->size() << " bytes)" << std::endl;
        entry.nextLink->forwardPacket(packet, this);
        break;
      }
    }

    if (!found) {
      std::cout << "Router #" << id() << ": no route for packet (from: " << packet->srcAddress().toString()
                << ", to: " << packet->destAddress().toString() << packet->size() << " bytes)" << std::endl;
      delete packet;
    }
  }
};

#endif