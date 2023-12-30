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
        using namespace Pistache::Rest;

        Routes::Get(router, "/", Routes::bind(&Service::ready, this));
        Routes::Post(router, "/chgen", Routes::bind(&Service::generateChangelog, this));
        Routes::Post(router, "/transfer", Routes::bind(&Service::generateTransferChangelog, this));
        Routes::Post(router, "/transfer/confirm", Routes::bind(&Service::transfer, this));
        Routes::Get(router, "/transfer/changelog", Routes::bind(&Service::lastTransferChangelog, this));
        Routes::Post(router, "/submit", Routes::bind(&Service::generateSubmissionChangelog, this));
        Routes::Post(router, "/submit/confirm", Routes::bind(&Service::submit, this));
        Routes::Get(router, "/submit/changelog", Routes::bind(&Service::lastSubmissionChangelog, this));
    }

    void Service::logRequest(const Request &request) {
        log("Received request from " + request.address().host() + ":" + std::to_string(request.address().port()) + " " +
            Pistache::Http::methodString(request.method()) + " " + request.resource());
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

    void Service::generateChangelog(const Service::Request &request, Service::Response response) {
        try {
            logRequest(request);

            const std::string& body = request.body();
            log("Raw content :\n" + body);

            rapidjson::Document document;
            document.Parse(body.c_str());

            if (!(document.HasMember("base_path") && document.HasMember("modified_path"))) {
                error("Incorrect arguments");
                response.send(Pistache::Http::Code::Bad_Request);
                return;
            }

            const std::string base_path = document["base_path"].GetString();
            const std::string modified_path = document["modified_path"].GetString();

            log("Parameter base_path : " + base_path);
            log("Parameter modified_path : " + modified_path);

            const auto changelog = chgen::ChangelogGenerator::scan(base_path, modified_path);
            if (changelog == nullptr) {
                response.send(Pistache::Http::Code::Bad_Request, "Could not generate changelog", MIME(Text, Plain));
                return;
            }

            chgen::ChangelogGenerator::generate(changelog);

            rapidjson::StringBuffer sb;
            rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

            changelog->Serialize(writer);

            const std::string string_changelog = sb.GetString();
            response.send(Pistache::Http::Code::Ok, string_changelog, MIME(Application, Json));
        } catch (const std::runtime_error &e) {
            log("Error: " + std::string(e.what()));
            response.send(Pistache::Http::Code::Not_Found, e.what(), MIME(Text, Plain));
        } catch (const std::exception &e) {
            log("Error: " + std::string(e.what()));
            response.send(Pistache::Http::Code::Internal_Server_Error, e.what(), MIME(Text, Plain));
        }
    }

    void Service::generateTransferChangelog(const Service::Request &request, Service::Response response) {
        try {
            logRequest(request);

            const std::string& body = request.body();
            log("Raw content :\n" + body);

            rapidjson::Document document;
            document.Parse(body.c_str());

            if (!(document.HasMember("unmodified_copy_path") && document.HasMember("modified_copy_path"))) {
                error("Incorrect arguments");
                response.send(Pistache::Http::Code::Bad_Request);
                return;
            }

            const std::string unmodified_copy_path = document["unmodified_copy_path"].GetString();
            const std::string modified_copy_path = document["modified_copy_path"].GetString();

            log("Parameter unmodified_copy_path : " + unmodified_copy_path);
            log("Parameter modified_copy_path : " + modified_copy_path);

            const auto changelog = transfer::DevbuildTransferer::getTransferChangelog(unmodified_copy_path, modified_copy_path);
            if (changelog == nullptr) {
                response.send(Pistache::Http::Code::Bad_Request, "Could not generate changelog", MIME(Text, Plain));
                return;
            }

            rapidjson::StringBuffer sb;
            rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

            changelog->Serialize(writer);

            const std::string string_changelog = sb.GetString();
            response.send(Pistache::Http::Code::Ok, string_changelog, MIME(Application, Json));
        } catch (const std::runtime_error &e) {
            log("Error: " + std::string(e.what()));
            response.send(Pistache::Http::Code::Not_Found, e.what(), MIME(Text, Plain));
        } catch (const std::exception &e) {
            log("Error: " + std::string(e.what()));
            response.send(Pistache::Http::Code::Internal_Server_Error, e.what(), MIME(Text, Plain));
        }
    }

    void Service::transfer(const Service::Request &request, Service::Response response) {
        try {
            logRequest(request);

            const std::string& body = request.body();
            log("Raw content :\n" + body);

            rapidjson::Document document;
            document.Parse(body.c_str());

            if (!document.HasMember("destination_path")) {
                error("Incorrect arguments");
                response.send(Pistache::Http::Code::Bad_Request);
                return;
            }

            const std::string destination_path = document["destination_path"].GetString();

            log("Parameter destination_path : " + destination_path);

            transfer::DevbuildTransferer::transfer(destination_path);
            transfer::DevbuildTransferer::exportChangelog();

            response.send(Pistache::Http::Code::Ok);
        } catch (const std::runtime_error &e) {
            log("Error: " + std::string(e.what()));
            response.send(Pistache::Http::Code::Not_Found, e.what(), MIME(Text, Plain));
        } catch (const std::exception &e) {
            log("Error: " + std::string(e.what()));
            response.send(Pistache::Http::Code::Internal_Server_Error, e.what(), MIME(Text, Plain));
        }
    }

    void Service::lastTransferChangelog(const Service::Request &request, Service::Response response) {
        try {
            logRequest(request);

            const auto changelog = transfer::DevbuildTransferer::getTransferChangelog();
            if (changelog == nullptr) {
                response.send(Pistache::Http::Code::Bad_Request, "Could not generate changelog", MIME(Text, Plain));
                return;
            }

            rapidjson::StringBuffer sb;
            rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

            changelog->Serialize(writer);

            const std::string string_changelog = sb.GetString();
            response.send(Pistache::Http::Code::Ok, string_changelog, MIME(Application, Json));
        } catch (const std::runtime_error &e) {
            log("Error: " + std::string(e.what()));
            response.send(Pistache::Http::Code::Not_Found, e.what(), MIME(Text, Plain));
        } catch (const std::exception &e) {
            log("Error: " + std::string(e.what()));
            response.send(Pistache::Http::Code::Internal_Server_Error, e.what(), MIME(Text, Plain));
        }
    }

    void Service::generateSubmissionChangelog(const Service::Request &request, Service::Response response) {
        try {
            logRequest(request);

            const std::string& body = request.body();
            log("Raw content :\n" + body);

            rapidjson::Document document;
            document.Parse(body.c_str());

            if (!(document.HasMember("unmodified_copy_path") && document.HasMember("modified_copy_path"))) {
                error("Incorrect arguments");
                response.send(Pistache::Http::Code::Bad_Request);
                return;
            }

            const std::string unmodified_copy_path = document["unmodified_copy_path"].GetString();
            const std::string modified_copy_path = document["modified_copy_path"].GetString();
            std::string archive_path;

            log("Parameter unmodified_copy_path : " + unmodified_copy_path);
            log("Parameter modified_copy_path : " + modified_copy_path);

            if(document.HasMember("archive_path")) {
                archive_path = document["archive_path"].GetString();
                log("Parameter archive_path : " + archive_path);
            }

            const auto changelog = submit::SubmissionBuilder::getSubmissionChangelog(unmodified_copy_path, modified_copy_path);
            if (changelog == nullptr) {
                response.send(Pistache::Http::Code::Bad_Request, "Could not generate changelog", MIME(Text, Plain));
                return;
            }

            rapidjson::StringBuffer sb;
            rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

            changelog->Serialize(writer);

            const std::string string_changelog = sb.GetString();
            response.send(Pistache::Http::Code::Ok, string_changelog, MIME(Application, Json));
        } catch (const std::runtime_error &e) {
            log("Error: " + std::string(e.what()));
            response.send(Pistache::Http::Code::Not_Found, e.what(), MIME(Text, Plain));
        } catch (const std::exception &e) {
            log("Error: " + std::string(e.what()));
            response.send(Pistache::Http::Code::Internal_Server_Error, e.what(), MIME(Text, Plain));
        }
    }

    void Service::submit(const Service::Request &request, Service::Response response) {
        try {
            logRequest(request);

            const std::string& body = request.body();
            log("Raw content :\n" + body);

            rapidjson::Document document;
            document.Parse(body.c_str());

            std::string archive_path;

            if(document.HasMember("archive_path")) {
                archive_path = document["archive_path"].GetString();
                log("Parameter archive_path : " + archive_path);

                submit::SubmissionBuilder::submit(archive_path);
            } else {
                submit::SubmissionBuilder::submit();
            }

            response.send(Pistache::Http::Code::Ok);
        } catch (const std::runtime_error &e) {
            log("Error: " + std::string(e.what()));
            response.send(Pistache::Http::Code::Not_Found, e.what(), MIME(Text, Plain));
        } catch (const std::exception &e) {
            log("Error: " + std::string(e.what()));
            response.send(Pistache::Http::Code::Internal_Server_Error, e.what(), MIME(Text, Plain));
        }
    }

    void Service::lastSubmissionChangelog(const Service::Request &request, Service::Response response) {
        try {
            logRequest(request);

            const auto changelog = submit::SubmissionBuilder::getSubmissionChangelog();
            if (changelog == nullptr) {
                response.send(Pistache::Http::Code::Bad_Request, "Could not generate changelog", MIME(Text, Plain));
                return;
            }

            rapidjson::StringBuffer sb;
            rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

            changelog->Serialize(writer);

            const std::string string_changelog = sb.GetString();
            response.send(Pistache::Http::Code::Ok, string_changelog, MIME(Application, Json));
        } catch (const std::runtime_error &e) {
            log("Error: " + std::string(e.what()));
            response.send(Pistache::Http::Code::Not_Found, e.what(), MIME(Text, Plain));
        } catch (const std::exception &e) {
            log("Error: " + std::string(e.what()));
            response.send(Pistache::Http::Code::Internal_Server_Error, e.what(), MIME(Text, Plain));
        }
    }

} // CUSubmitterService

