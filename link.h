#ifndef LINK_H
#define LINK_H

#include "node.h"
#include "object.h"
#include "packet.h"
#include <cstdlib>

class Link : public Object {
  friend class LinkInstaller;

  private:
  Link(Node *nodeA, Node *nodeB, double delay = 0.0)
      : nodeA_(nodeA), nodeB_(nodeB), delay_(delay) {}

  std::string name() override { return "Link"; }
  
  Node *nodeA_;
  Node *nodeB_;
  double delay_;

  public:
  // 매개변수로 주어진 노드가 아닌 반대편 노드를 구한다.
  Node *other(const Node *node) const { return node == nodeA_ ? nodeB_ : nodeA_; }

  Node *nodeA() { return nodeA_; }
  Node *nodeB() { return nodeB_; }
  double delay() { return delay_; }

  void forwardPacket(Packet *packet, Node *srcNode) {
    Node *destNode = other(srcNode);
    std::string packetID = packet->toString();
    std::string destNodeID = destNode->toString();
    std::string fromNodeID = srcNode->toString();
    log("packet in: " + packetID + " from " + fromNodeID);
    
    // [this, packet, destNode]는 람다가 생성될 때 생성, packet과 destNode는 포인터이므로 '참조'로 전달
    Simulator::schedule(Simulator::now() + delay_, [this, packet, destNode, packetID, destNodeID]() {
        log("packet out: " + packetID + " to " + destNodeID);
        destNode->receive(packet);
    });
  }
};

#endif