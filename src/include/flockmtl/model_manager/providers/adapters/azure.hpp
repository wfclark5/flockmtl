#pragma once

#include "flockmtl/model_manager/providers/provider.hpp"
#include "flockmtl/model_manager/providers/handlers/azure.hpp"

namespace flockmtl {

class AzureProvider : public IProvider {
public:
    AzureProvider(const ModelDetails &model_details) : IProvider(model_details) {}

    nlohmann::json CallComplete(const std::string &prompt, bool json_response) override;
    nlohmann::json CallEmbedding(const std::vector<std::string> &inputs) override;
};

} // namespace flockmtl
