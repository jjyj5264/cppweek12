#ifndef LINK_INSTALLER_H
#define LINK_INSTALLER_H

#include "link.h"
#include "service.h"

class LinkInstaller {
  public:
  // 노드 a, b 사이에 링크를 설치하고 반환한다.
  Link *install(Node *nodeA, Node *nodeB, double delay = 0.0) {
    Link *link = new Link(nodeA, nodeB, delay);
    nodeA->addLink(link);
    nodeB->addLink(link);
    
    return link;
  }
};

#endif