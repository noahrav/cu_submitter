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

        response.send(Http::Code::Ok, "CU Submitter backend is reponding\n");
    }
};

#endif //CU_SUBMITTER_API_H