#pragma once

#include <string>

namespace flockmtl {

enum class PromptSection { USER_PROMPT, TUPLES, RESPONSE_FORMAT, INSTRUCTIONS };

enum class AggregateFunctionType { REDUCE, FIRST, LAST, RERANK };

enum class ScalarFunctionType { COMPLETE_JSON, COMPLETE, FILTER };

constexpr auto META_PROMPT =
    "You are a semantic analysis tool for DBMS. The tool will analyze each tuple in the provided data and respond to "
    "user requests based on this context.\n\nUser Prompt:\n\n- {{USER_PROMPT}}\n\nTuples "
    "Table:\n\n{{TUPLES}}\n\nInstructions:\n\n{{INSTRUCTIONS}}\n\nExpected Response Format:\n\n{{RESPONSE_FORMAT}}";

class INSTRUCTIONS {
public:
    static constexpr auto SCALAR_FUNCTION =
        "- The response should be directly relevant to each tuple without additional formatting, purely answering the "
        "prompt as if each tuple were a standalone entity.\n- Use clear, context-relevant language to generate a "
        "meaningful and concise answer for each tuple.";
    static constexpr auto AGGREGATE_FUNCTION =
        "- For each tuple in the provided data, evaluate the relevant attribute(s) based on the user prompt.\n- After "
        "evaluating each tuple, aggregate the results according to the user's query. This could involve different "
        "operations depending on the prompt.\n- The aggregation should summarize the responses of all tuples into a "
        "single final result that answers the user's question as a whole.\n- The final result should be returned "
        "according to the expected response format.";

    template <typename FunctionType>
    static std::string Get(FunctionType option);
};

class RESPONSE_FORMAT {
public:
    // Scalar Functions
    static constexpr auto COMPLETE_JSON =
        "The system should interpret database tuples and provide a response to the user's prompt for each tuple in a "
        "JSON format that contains the necessary columns for the answer.\n\nThe tool should respond in JSON format as "
        "follows:\n\n```json\n{\t\"tuples\": [\n\t\t{<response 1>},\n\t\t{<response 2>},\n\t\t...\n\t\t{<response "
        "n>}\n\t]\n}\n```. \n\n Rules: The system should not ask for clarifying questions, just return the requested "
        "JSON format";
    static constexpr auto COMPLETE =
        "The system should \n 1) for each database tuple interpret it individually \n 2) for each tuple provide a "
        "response to the user's prompt for each tuple in "
        "plain text.\n\t 3) The tool should respond in JSON format as follows:\n\n```json\n{\"tuples\": [\"<response "
        "1>\", \"<response 2>\", ... , \"<response n>\"]}\n``` Response Rules:\n Do not ask for clarifying questions "
        "just return the JSON response.\n Do not add any additional text or explanation beyond the JSON response.\n Do "
        "not include any markdown formatting in the response.\n";
    static constexpr auto FILTER =
        "The system should interpret database tuples and provide a response to the user's prompt for each tuple in a "
        "BOOL format that would be true/false.\n\tThe tool should respond in JSON format as "
        "follows:\n\n```json\n{\"tuples\": [<bool response 1>, <bool response 2>, ... , <bool response n>]}";

    // Aggregate Functions
    static constexpr auto REDUCE =
        "Return a single, coherent output that synthesizes the most relevant information from the tuples provided. "
        "Respond in the following JSON format. **Do not add explanations or additional words beyond the summarized "
        "content.**\n\nResponse Format:\n\n```json\n{\n  \"output\": <summarized content here>\n}\n```";
    static constexpr auto FIRST_OR_LAST =
        "Identify the {{RELEVANCE}} relevant tuple based on the provided user prompt from the list of tuples. Output "
        "the index of this single tuple as a JSON object in the following format:\n\n```json\n{\n  \"selected\": "
        "flockmtl_tuple_id\n}\n```";
    static constexpr auto RERANK =
        "Rank the tuples in descending order of relevance according to the user prompt. Output the response strictly "
        "as a JSON object in the following format:\n\n```json\n{\n  \"ranking\": [flockmtl_tuple_id1, "
        "flockmtl_tuple_id2, flockmtl_tuple_id3, ...]\n}\n```\n\nInclude all tuple IDs in the response, listed without "
        "any additional text or explanation.";

    template <typename FunctionType>
    static std::string Get(const FunctionType option);
};

struct PromptDetails {
    std::string prompt_name;
    std::string prompt;
    int version;
};

} // namespace flockmtl
