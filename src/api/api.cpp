#include "api.h"

namespace CUSubmitterService {

    Service::Service(Pistache::Address addr)
            : server(std::make_shared<Pistache::Http::Endpoint>(addr)),
              port(addr.port()) {
    }

    void Service::run(size_t thr) {
        log("Starting server on port " + port.toString() + " with " + std::to_string(thr) + " threads");

        auto opts = Pistache::Http::Endpoint::options().threads(static_cast<int>(thr));

        server->init(opts);

        configureRoutes();
        server->setHandler(router.handler());

        server->serve();
    }

    void Service::shutdown() {
        log("Shutting server down");

        server->shutdown();
    }

    void Service::configureRoutes() {
        Pistache::Rest::Routes::Get(router, "/", Pistache::Rest::Routes::bind(&Service::ready, this));
    }

    void Service::ready(const Request &request, Response response) {
        try {
            logRequest(request);
            response.send(Pistache::Http::Code::Ok, "CU Submitter is up and running\n", MIME(Text, Plain));
        } catch (const std::runtime_error &e) {
            log("Error: " + std::string(e.what()));
            response.send(Pistache::Http::Code::Not_Found, e.what(), MIME(Text, Plain));
        } catch (const std::exception &e) {
            log("Error: " + std::string(e.what()));
            response.send(Pistache::Http::Code::Internal_Server_Error, e.what(), MIME(Text, Plain));
        }
    }

    void Service::logRequest(const Request &request) {
        log("Received request from " + request.address().host() + ":" + std::to_string(request.address().port()) + " " +
            Pistache::Http::methodString(request.method()) + " " + request.resource());
    }

} // CUSubmitterService

