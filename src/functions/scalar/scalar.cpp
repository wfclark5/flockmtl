#include "flockmtl/functions/scalar/scalar.hpp"

namespace flockmtl {

/*------------------------------------------------------------------*/
/*  COMPLETE (single prompt)                                        */
/*------------------------------------------------------------------*/
nlohmann::json ScalarFunctionBase::Complete(const nlohmann::json& tuples, const std::string& user_prompt,
                                            ScalarFunctionType function_type, Model& model) {
    const auto prompt = PromptManager::Render(user_prompt, tuples, function_type);
    auto response = model.CallComplete(prompt);
    std::cout << "Response size: " << response.dump() << std::endl;
    return response["tuples"];
}

/*------------------------------------------------------------------*/
/*  BATCH + COMPLETE                                                */
/*------------------------------------------------------------------*/
nlohmann::json ScalarFunctionBase::BatchAndComplete(const std::vector<nlohmann::json>& tuples,
                                                    const std::string& user_prompt,
                                                    const ScalarFunctionType function_type, Model& model) {
    /* ---------- pre-compute context-window room ----------------- */
    auto tmpl = PromptManager::GetTemplate(function_type);
    auto meta_tokens = Tiktoken::GetNumTokens(user_prompt) + Tiktoken::GetNumTokens(tmpl);
    auto available_tokens = model.GetModelDetails().context_window - meta_tokens;

    if (available_tokens < 0) {
        throw std::runtime_error("Prompt exceeds the model context window");
    }

    nlohmann::json responses = nlohmann::json::array();
    /* ---------- batching loop ----------------------------------- */
    std::size_t start_index = 0;
    std::size_t batch_size = tuples.size(); // initial guess
    nlohmann::json batch_tuples = nlohmann::json::array();

    while (start_index < tuples.size()) {
        /* clear & rebuild batch */
        std::size_t accumulated = 0;
        batch_tuples.clear();

        while (start_index < tuples.size()) {
            const auto& candidate = tuples[start_index];
            auto cand_tokens = Tiktoken::GetNumTokens(PromptManager::ConstructMarkdownSingleTuple(candidate));

            /* -----------------------------------------------------
             * If **one tuple alone** would overflow the context
             * window, we cannot send it to the model as-is.
             * Instead, record a NULL for that row and move on.
             * --------------------------------------------------- */
            if (cand_tokens > static_cast<unsigned>(available_tokens)) {
                responses.push_back(nullptr);
                ++start_index;
                continue; // try next tuple
            }

            /* Would adding this tuple overflow the current batch? */
            if (accumulated + cand_tokens > static_cast<unsigned>(available_tokens) ||
                batch_tuples.size() >= batch_size) {
                break; // finish this batch
            }

            batch_tuples.push_back(candidate);
            accumulated += cand_tokens;
            ++start_index;
        }

        /* no tuples fit -> continue (all were oversized & nulled) */
        if (batch_tuples.empty()) {
            continue;
        }

        /* -------- call the model for this batch ----------------- */
        nlohmann::json batch_response;
        try {
            batch_response = Complete(batch_tuples, user_prompt, function_type, model);
            std::cout << "Batch response size: " << batch_response.size() << std::endl;
        } catch (const ExceededMaxOutputTokensError&) {
            /* Back off to smaller batches and retry --------------- */
            batch_size = std::max<std::size_t>(1, batch_size / 2);
            continue; // rebuild batch
        }

        /* -------- pad / trim so cardinalities match ------------- */
        if (batch_response.size() < batch_tuples.size()) {
            batch_response.insert(batch_response.end(), batch_tuples.size() - batch_response.size(), nullptr);
        } else if (batch_response.size() > batch_tuples.size()) {
            batch_response.erase(batch_response.begin() + batch_tuples.size(), batch_response.end());
        }

        /* -------- accumulate results --------------------------- */
        for (auto& row : batch_response)
            responses.push_back(row);

        /* -------- recompute next-batch hint --------------------- */
        auto tokens_per_tuple =
            std::max<unsigned>(1u, Tiktoken::GetNumTokens(batch_response.dump()) / batch_tuples.size());

        batch_size = std::max<std::size_t>(1, model.GetModelDetails().max_output_tokens / tokens_per_tuple);
    }

    return responses;
}

} // namespace flockmtl
