#ifndef CU_SUBMITTER_API_H
#define CU_SUBMITTER_API_H

#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "../chgen/chgen.h"
#include "../data/changelog.h"
#include "../transfer/transfer.h"
#include "../submit/submit.h"
#include "../utils/log.h"

namespace CUSubmitterService {

    class Service {
    public:
        explicit Service(Pistache::Address addr);

        void run(size_t thr = std::thread::hardware_concurrency());

        void shutdown();

    private:
        void configureRoutes();

        using Request = Pistache::Rest::Request;
        using Response = Pistache::Http::ResponseWriter;

        void ready(const Request& request, Response response);
        void generateChangelog(const Request& request, Response response);
        void generateTransferChangelog(const Request& request, Response response);
        void transfer(const Request& request, Response response);
        void lastTransferChangelog(const Request& request, Response response);
        void generateSubmissionChangelog(const Request& request, Response response);
        void submit(const Request& request, Response response);
        void lastSubmissionChangelog(const Request& request, Response response);

        static void logRequest(const Request& request);

        std::shared_ptr<Pistache::Http::Endpoint> server;
        Pistache::Rest::Router router;
        Pistache::Port port;
    };

} // CUSubmitterService

#endif //CU_SUBMITTER_API_H