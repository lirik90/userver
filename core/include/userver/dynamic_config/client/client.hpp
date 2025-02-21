#pragma once

/// @file userver/dynamic_config/client/client.hpp
/// @brief @copybrief dynamic_config::Client

#include <chrono>
#include <optional>

#include <userver/dynamic_config/value.hpp>

USERVER_NAMESPACE_BEGIN

namespace clients::http {
class Client;
}  // namespace clients::http

namespace dynamic_config {

struct ClientConfig {
    std::string service_name;
    bool get_configs_overrides_for_service{true};
    std::chrono::milliseconds timeout{0};
    int retries{1};
    std::string config_url;
    bool append_path_to_url{true};
    std::string stage_name;
    bool fallback_to_no_proxy{true};
};

/// @ingroup userver_clients
///
/// @brief Client for the
/// @ref scripts/docs/en/userver/dynamic_config.md "dynamic configs" service.
///
/// It is safe to concurrently invoke members of the same client because this
/// client is a thin wrapper around clients::http::Client.
class Client final {
public:
    Client(clients::http::Client& http_client, const ClientConfig&);

    ~Client();

    using Timestamp = std::string;

    struct Reply {
        USERVER_NAMESPACE::dynamic_config::DocsMap docs_map;
        std::vector<std::string> kill_switches_disabled;
        std::vector<std::string> removed;
        Timestamp timestamp;

        bool IsEmpty() const { return docs_map.Size() == 0 && removed.empty(); }
    };

    struct JsonReply {
        formats::json::Value configs;
        std::vector<std::string> kill_switches_disabled;
        Timestamp timestamp;
    };

    Reply DownloadFullDocsMap();

    Reply FetchDocsMap(const std::optional<Timestamp>& last_update, const std::vector<std::string>& fields_to_load);

    JsonReply FetchJson(const std::optional<Timestamp>& last_update, const std::vector<std::string>& fields_to_load);

private:
    formats::json::Value
    FetchConfigs(const std::optional<Timestamp>& last_update, const std::vector<std::string>& fields_to_load);

    std::string FetchConfigsValues(std::string_view body);

    const ClientConfig config_;
    clients::http::Client& http_client_;
};

}  // namespace dynamic_config

USERVER_NAMESPACE_END
