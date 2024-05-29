#ifndef NODE_H
#define NODE_H

#include "packet.h"
#include <vector>

class Link;

class Node {
  friend class LinkInstaller;

  private:
  int id_;
  static int nextId_;

  protected:
  std::vector<Link *> links_; // 노드에 연결된 링크 목록

  public:
  Node() : id_(nextId_++) {}

  int id() const { return id_; }

  // 링크를 추가하는 메서드
  void addLink(Link *link) {
    links_.push_back(link);
  }

  virtual void receive(Packet *packet) = 0;
};

#endif