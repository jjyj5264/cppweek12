#ifndef BULK_SEND_SERVICE_INSTALLER_H
#define BULK_SEND_SERVICE_INSTALLER_H

#include "address.h"
#include "bulk_send_service.h"
#include "service_installer.h"

class BulkSendServiceInstaller : public ServiceInstaller {
  // 얘는 1주차 installer와는 다르게 생성자가 없다?
  public:
  BulkSendService *install(Host *host, Address destination, short destPort, double delay = 1, double startTime = 0, double stopTime = 10.) {
    BulkSendService *bulkSendService = new BulkSendService(host, destination, destPort, delay, startTime, stopTime);
    ServiceInstaller::install(host, bulkSendService);

    for (double time = startTime; time < stopTime; time += delay) {
      Simulator::schedule(time, [bulkSendService, host, destination, destPort]() {
        bulkSendService->bulkSend(host, destination, destPort);
      });
    }

    return bulkSendService;
  }
};

#endif