#ifndef CU_SUBMITTER_API_H
#define CU_SUBMITTER_API_H

#include <pistache/endpoint.h>

using namespace Pistache;

class CUSubmitterHandler : public Http::Handler {
public:
    HTTP_PROTOTYPE(CUSubmitterHandler)

    void onRequest(const Http::Request& request, Http::ResponseWriter response) {        
        response.send(Http::Code::Ok, "CU Submitter backend is reponding\n");
    }
};

#endif //CU_SUBMITTER_API_H