#ifndef CU_SUBMITTER_API_H
#define CU_SUBMITTER_API_H

#include <pistache/endpoint.h>
#include "../utils/log.h"

using namespace Pistache;

class CUSubmitterHandler : public Http::Handler {
public:
    HTTP_PROTOTYPE(CUSubmitterHandler)

    static void logRequest(const Http::Request& request) {
        log("Request received : " + request.resource() + " " + Http::methodString(request.method()) + " from " + request.address().host() + ":" + std::to_string(request.address().port()));
        if (request.body().empty()) {
            log("No content");
        } else {
            log("Content : " + request.body());
        }
    }

    void onRequest(const Http::Request& request, Http::ResponseWriter response) override {
        logRequest(request);

        if (request.resource() == "/" && request.method() == Http::Method::Get) {
            response.send(Http::Code::Ok, "CU Submitter backend is reponding\n");
        } else {
            response.send(Http::Code::Not_Found);
        }
    }
};

class CUSubmitterService {
public:
    explicit CUSubmitterService(Address addr)
            : server(std::make_shared<Http::Endpoint>(addr)) {}

    void init(size_t thr = 2) {
        auto opts = Http::Endpoint::options().threads(static_cast<int>(thr));

        server->init(opts);
        server->setHandler(Http::make_handler<CUSubmitterHandler>());
    }

    void start() {
        server->serve();
    }

    void shutdown() {
        server->shutdown();
    }

private:
    std::shared_ptr<Http::Endpoint> server;
};

#endif //CU_SUBMITTER_API_H