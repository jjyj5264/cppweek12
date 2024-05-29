#ifndef MANUAL_ROUTER_H
#define MANUAL_ROUTER_H

#include "address.h"
#include "router.h"

class ManualRouter : public Router {

  public:
  // 목적지 주소에 따른 다음 링크를 설정한다.
  // destination으로 가려면 -> nextLink로 가라.
  void addRoutingEntry(const Address &destination, Link *nextLink) {
    RoutingEntry entry{destination, nextLink};
    routingTable_.push_back(entry);
  }

  virtual ~ManualRouter() {}

  // void recieve() 함수는 자신의 부모 클래스인 router 클래스의 것을 사용한다.
  void receive(Packet *packet) {
    Router::receive(packet);
  }
};

#endif