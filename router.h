#ifndef ROUTER_H
#define ROUTER_H

#include "node.h"

struct RoutingEntry {
  public:
  Address destination;
  Link *nextLink;
};

class Router : public Node {
  protected:
  std::vector<RoutingEntry> routingTable_;

  public:
  void receive(Packet *packet) {
    std::string packetID = packet->toString();
    Address destAddress = packet->destAddress();
    bool found = false;

    for (RoutingEntry &entry : routingTable_) {
      if (entry.destination == destAddress) {
        std::string linkID = entry.nextLink->toString();
        log("forwarding packet: " + packetID + " to " + linkID); // Router는 Packet을 Link에 전달, Link는 Node(Host, Router...)에 전달
        entry.nextLink->forwardPacket(packet, this);
        found = true;
        break;
      }
    }

    if (!found) {
      log("no route for packet: " + packetID);
      delete packet;
    }
  }
};

#endif