#include "host.h"
#include "link.h"
#include "router.h"
#include "service.h"
#include <algorithm>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

struct Distance {
  double delay_;
  Node *node_;
  Link *link_;

  Distance(double delay, Node *node, Link *link) : delay_(delay), node_(node), link_(link) {}

  // 지연 시간이 더 큰 것이 낮은 우선순위를 가짐
  bool operator<(const Distance &s) const {
    return this->delay_ > s.delay_;
  }
};

class AutoRouter : public Router {
  private:
  virtual std::string name() override { return "AutoRouter"; }

  public:
  void calculate(const std::vector<Node *> &nodes, const std::vector<Link *> &links) {
    std::unordered_map<Node *, double> delayMap; // Node별 delay
    std::unordered_map<Node *, Link *> linkMap;  // Node별 Link
    std::unordered_map<Node *, bool> hasVisited; // Node 방문 여부

    // 초기화
    for (auto &node : nodes) {
      delayMap[node] = std::numeric_limits<double>::infinity(); // 무한대로 초기화
      linkMap[node] = nullptr;
      hasVisited[node] = false; // 아직 방문하지 않음
    }

    delayMap[this] = 0.0; // 자기 자신과의 delay는 0

    std::priority_queue<Distance> queue;

    // 시작 Node에 연결된 Link들에 대한 초기 거리 계산 및 우선순위 큐에 추가
    for (auto &link : this->links_) {
      Node *other = link->other(this);
      delayMap[other] = link->delay();
      linkMap[other] = link;
      queue.push(Distance(delayMap[other], other, linkMap[other]));
    }

    while (!queue.empty()) { // 모든 노드에 대해
      Distance distance = queue.top();
      queue.pop();

      // 이미 방문한 노드에 대해서는 pass
      if (hasVisited[distance.node_]) { continue; }

      hasVisited[distance.node_] = true; // 해당 노드를 방문한 것으로 처리

      // 현재 Node에 연결된 '모든' Link의 경로 업데이트
      for (auto &link : distance.node_->links_) {
        double totalDelay = distance.delay_ + link->delay();
        Node *targetNode = link->other(distance.node_);

        // 더 짧은 경로를 찾은 경우
        if (delayMap[targetNode] > totalDelay) {
          delayMap[targetNode] = totalDelay;
          linkMap[targetNode] = distance.link_; // 최단 경로의 첫 Link 저장
          queue.push(Distance(delayMap[targetNode], targetNode, linkMap[targetNode]));
        }
      }
    }

    // 라우팅 테이블 업데이트
    this->routingTable_.clear();
    for (auto &iterator : linkMap) {
      Node *node = iterator.first;
      Link *link = iterator.second;

      if (link == nullptr || node == this) {
        continue; // 시작 Node와 연결되지 않은 Node는 pass
      }

      if (Host *host = dynamic_cast<Host *>(node)) { // node(iterator.first)는 Host인가
        this->routingTable_.push_back({host->address(), link});
      }
    }
  }
};