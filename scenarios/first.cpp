#include "../echo_service.h"
#include "../echo_service_installer.h"
#include "../host.h"
#include "../link_installer.h"
#include "../manual_router.h"
#include "../message_service_installer.h"
#include <vector>

#define ECHO_PORT 3000

// # 네트워크 시뮬레이터
// 네트워크 시뮬레이터를 구현하고자 한다.
// ## 클래스
// ### Node
// 링크로 연결할 수 있는 장치를 나타낸 클래스다. id를 가지며 Node를 생성할 때마다 1씩 증가한다. (id는 이미 구현돼있다.)
// ### Host
// 우리가 일반적으로 사용하는 컴퓨터와 같이 네트워킹이 가능한 기기를 나타낸 클래스다. 하나의 주소를 가지며 여러 링크를 가지지만, 패킷 전송 시 하나의 링크를 랜덤으로 골라 전송한다. 패킷을 전송하거나 받을때 다음과 같이 출력한다.
// Host #1: sending packet (from: 0, to: 1, 13 bytes)
// Host #0: received packet, destination port: 3000
// 포트에 맞는 서비스가 없다면 다음과 같이 출력한다.
// Host #0: received packet, destination port: 0
// Host #0: no service for packet (from: 456, to: 123, 7 bytes)
// ### Service
// Host에 설치된 프로그램을 나타낸 클래스다. 하나의 포트를 사용하며, 패킷을 생성하여 전송하거나 자신에게 온 패킷을 처리한다.
// ### Router
// 라우터를 나타낸 클래스다. 패킷을 받으면 라우팅 테이블에 따라 패킷을 다음 링크로 전송한다. 패킷을 포워딩할 때 다음과 같이 출력한다.
// Router #2: forwarding packet (from: 1, to: 0, 13 bytes)
// 라우팅 테이블에 존재하지 않는 주소를 목적 주소로 하는 패킷이 온 경우, 다음 같이 출력한다.
// Router #6: no route for packet (from: 456, to: 0, 7 bytes)
// ### ManualRouter
// 라우팅 테이블을 직접 설정할 수 있는 클래스다.
// ### Address
// Host의 주소를 저장하는 클래스다.
// ### Link
// Node를 잇는 경로다. 한 Node에서 패킷을 받아 다른 Node로 전달한다. 패킷을 전달할 때 보낸 쪽의 노드와 받는 쪽의 노드 id와 함께 다음과 같이 메시지를 출력한다.
// Link: forwarding packet from node #1, to node #5
// ### ...Installer
// Link와 각 Service를 설치하는 것에 도움을 주는 Helper 클래스다.
// ### EchoService
// 받은 패킷을 전송자에게 그대로 다시 전달하는 Service를 나타내는 클래스다. 받은 패킷을 다음과 같이 출력한다.
// EchoService: received "Hello, world!" from 0:1000, send reply with same data
// ### MessageService
// send 함수를 호출하여 문자열 데이터를 네트워크로 전송할 수 있는 Service를 나타낸 클래스다. 받은 패킷을 다음과 같이 출력한다.
// MessageService: received "Hello, world!" from 1:3000
// 1. 서버의 포트는 Service 측에서 설정합니다. 다만 클라이언트의 경우 사용 가능한 포트(아무 서비스도 사용하지 않는 포트)를 아무렇게나 골라서 사용합니다. 출력 예시의 경우 1000번 포트부터 1씩 증가시키며 사용 가능한 포트를 찾아서 사용하도록 했습니다.
// 2. 따라서 출력 예시의 Client 측 포트는 동일하지 않아도 됩니다. 다만 여러 서비스가 동일한 포트를 사용해서는 안됩니다.
// 3. Installer 클래스의 경우 시나리오를 작성하는 입장에서 객체를 올바르지 않게 생성하고 설정하는 것을 방지하기 위한 클래스입니다. 예를 들어 Service와 Host 간의 참조 관계가 있는데, 이를 위한 포인터가 잘못 설정되는 경우 문제가 될 수 있습니다. 이것을 아예 방지하기 위해 객체 생성과 포인터 설정을 Service와 Host에서 은닉하고(private으로 설정), Installer 클래스가 하나의 함수로 노출하도록(public으로 설정) 한 것입니다. 이는 객체지향 요소 중 캡슐화를 고려한 설계입니다.
// 4. 패킷은 다음 과정을 걸쳐서 전달됩니다: Service -> Host -> Link -> Router -> Link -> Router -> Link -> Host -> Service

int main() {
  // ---------- //
  // 토폴로지 설정 //
  // ---------- //

  // 호스트를 생성한다
  // !!! Node는 Address를 가지지 않는 것에 유의 !!!
  // !!! Node는 ID만 가지고 있음 !!!
  // !!! Host가 ID와 Address를 가지고 있음 !!!
  Host *echoServer = new Host(1);    // Address: 1
  Host *messageClient = new Host(0); // Address: 0

  // 서비스를 설치한다
  EchoServiceInstaller echoServiceInstaller(ECHO_PORT); // 해당 'Service'의 port를 3000으로 지정
  echoServiceInstaller.install(echoServer); // 해당 객체를 저장하고, Host::services_에 service를 등록
  MessageServiceInstaller messageServiceInstaller(echoServer->address(), ECHO_PORT); // destAddress: 1, destPort: 3000
  // messageService 객체를 생성하고, Host::services_에 service를 등록
  // install() 내부의 host, destPort_, destAddress_, destPort_는 MessageServiceInstaller 생성 시에 이미 존재
  MessageService *messageService = messageServiceInstaller.install(messageClient);
  // 현재, 각각의 Host에 서비스가 하나씩 설치되어 있음

  // 라우터를 생성한다.
  std::vector<ManualRouter *> routers;
  for (int i = 0; i < 4; i++) {
    routers.push_back(new ManualRouter());
  }

  // 라우터와 호스트 간에 링크로 연결한다.
  LinkInstaller linkInstaller;
  std::vector<Link *> links;
  links.push_back(linkInstaller.install(routers[0], echoServer));    // l0
  links.push_back(linkInstaller.install(routers[0], routers[1]));    // l1
  links.push_back(linkInstaller.install(routers[0], routers[2]));    // l2
  links.push_back(linkInstaller.install(routers[1], routers[3]));    // l3
  links.push_back(linkInstaller.install(routers[2], routers[3]));    // l4
  links.push_back(linkInstaller.install(routers[3], messageClient)); // l5

  // 라우팅 테이블을 설정한다.
  routers[0]->addRoutingEntry(echoServer->address(), links[0]);
  routers[0]->addRoutingEntry(messageClient->address(), links[1]);
  routers[0]->addRoutingEntry(messageClient->address(), links[2]);

  routers[1]->addRoutingEntry(echoServer->address(), links[1]);
  routers[1]->addRoutingEntry(messageClient->address(), links[3]);

  routers[2]->addRoutingEntry(echoServer->address(), links[2]);
  routers[2]->addRoutingEntry(messageClient->address(), links[4]);

  routers[3]->addRoutingEntry(echoServer->address(), links[3]);
  routers[3]->addRoutingEntry(echoServer->address(), links[4]);
  routers[3]->addRoutingEntry(messageClient->address(), links[5]);

  // 토폴로지는 다음 그림과 같다:
  //
  //   echoServer
  //       |-l0
  //     router0
  //   l1-/   |-l2
  // router1  router2
  //   l3-\   |-l4
  //     router3
  //       |-l5
  //  messageClient(client)

  // ------------ //
  // 시뮬레이션 수행 //
  // ------------ //

  // 각 호스트를 초기화한다.
  echoServer->initialize(); // 얘부터 초기화해야 안 쓰는 port를 찾아서 사용할 수 있음
  messageClient->initialize();

  // 메시지를 전송한다.
  messageService->send("Hello, world!"); // !!! Client가 아닌 Service에서 메시지를 전송하는 것에 유의 !!!
  messageService->send("Bye, world!");

  // --- //
  // 정리 //
  // --- //

  // 생성한 객체를 제거한다.
  for (size_t i = 0; i < links.size(); i++) {
    delete links[i];
  }

  for (size_t i = 0; i < routers.size(); i++) {
    delete routers[i];
  }

  delete echoServer;
  delete messageClient;
}