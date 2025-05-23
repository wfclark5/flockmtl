#include "flockmtl/model_manager/providers/adapters/openai.hpp"

namespace flockmtl {

nlohmann::json OpenAIProvider::CallComplete(const std::string& prompt, bool json_response) {
    auto base_url = std::string("");
    if (const auto it = model_details_.secret.find("base_url"); it != model_details_.secret.end()) {
        base_url = it->second;
    }
    auto openai = openai::OpenAI(model_details_.secret["api_key"], "", true, base_url);

    // Create a JSON request payload with the provided parameters
    nlohmann::json request_payload = {{"model", model_details_.model},
                                      {"messages", {{{"role", "user"}, {"content", prompt}}}},
                                      {"max_completion_tokens", model_details_.max_output_tokens},
                                      {"temperature", model_details_.temperature}};

    // Conditionally add "response_format" if json_response is true
    if (json_response) {
        request_payload["response_format"] = {{"type", "json_object"}};
    }

    // Make a request to the OpenAI API
    nlohmann::json completion;
    try {
        completion = openai.chat.create(request_payload);
    } catch (const std::exception& e) {
        throw std::runtime_error("Error in making request to OpenAI API: " + std::string(e.what()));
    }
    // Check if the conversation was too long for the context window
    if (completion["choices"][0]["finish_reason"] == "length") {
        // Handle the error when the context window is too long
        throw ExceededMaxOutputTokensError();
    }

    // Check if the OpenAI safety system refused the request
    if (completion["choices"][0]["message"]["refusal"] != nullptr) {
        // Handle refusal error
        throw std::runtime_error(
            duckdb_fmt::format("The request was refused due to OpenAI's safety system.{{\"refusal\": \"{}\"}}",
                               completion["choices"][0]["message"]["refusal"].get<std::string>()));
    }

    // Check if the model's output included restricted content
    if (completion["choices"][0]["finish_reason"] == "content_filter") {
        // Handle content filtering
        throw std::runtime_error("The content filter was triggered, resulting in incomplete JSON.");
    }

    std::string content_str = completion["choices"][0]["message"]["content"];

    if (json_response) {
        return nlohmann::json::parse(content_str);
    }

    return content_str;
}

nlohmann::json OpenAIProvider::CallEmbedding(const std::vector<std::string>& inputs) {
    auto base_url = std::string("");
    if (const auto it = model_details_.secret.find("base_url"); it != model_details_.secret.end()) {
        base_url = it->second;
    }
    auto openai = openai::OpenAI(model_details_.secret["api_key"], "", true, base_url);

    // Create a JSON request payload with the provided parameters
    nlohmann::json request_payload = {
        {"model", model_details_.model},
        {"input", inputs},
    };

    // Make a request to the OpenAI API
    auto completion = openai.embedding.create(request_payload);

    // Check if the conversation was too long for the context window
    if (completion["choices"][0]["finish_reason"] == "length") {
        // Handle the error when the context window is too long
        throw ExceededMaxOutputTokensError();
    }

    // Check if the OpenAI safety system refused the request
    if (completion["choices"][0]["message"]["refusal"] != nullptr) {
        return nlohmann::json {{"error", "refusal"},
                               {"reason", completion["choices"][0]["message"]["refusal"].get<std::string>()}};
    }

    auto embeddings = nlohmann::json::array();
    for (auto& item : completion["data"]) {
        embeddings.push_back(item["embedding"]);
    }

    return embeddings;
}

} // namespace flockmtl
