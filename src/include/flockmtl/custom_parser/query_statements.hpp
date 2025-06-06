#pragma once

#include "flockmtl/core/common.hpp"

#include <memory>
#include <string>
#include <vector>

namespace flockmtl {

// Enum to represent different statement types
enum class StatementType {
    CREATE_MODEL,
    DELETE_MODEL,
    UPDATE_MODEL,
    UPDATE_MODEL_SCOPE,
    GET_MODEL,
    GET_ALL_MODEL,
    CREATE_PROMPT,
    DELETE_PROMPT,
    UPDATE_PROMPT,
    UPDATE_PROMPT_SCOPE,
    GET_PROMPT,
    GET_ALL_PROMPT,
};

// Abstract base class for statements
class QueryStatement {
public:
    virtual ~QueryStatement() = default;
    StatementType type;
};

// Statement for 'CREATE DUCK' command
class CreateDuckStatement : public QueryStatement {
public:
    std::string duck_string;
};

} // namespace flockmtl