#include "flockmtl/model_manager/model.hpp"
#include "flockmtl/core/config.hpp"
#include "flockmtl/functions/scalar/llm_complete.hpp"
#include <duckdb.hpp>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

namespace flockmtl {
using json = nlohmann::json;

TEST(RealProviderIntegrationTest, OpenAIRealCallTest) {

    // Read the SQL file as text
    std::ifstream file("/home/situ/flockmtl/test/test.sql");
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sql = buffer.str();

    auto db = std::make_unique<duckdb::DuckDB>("flockmtl_test.db");

    auto con = Config::GetConnection(&*db->instance);
    // Register your OpenAI secret in the DuckDB instance
    con.Query("CREATE OR REPLACE SECRET (TYPE OPENAI, API_KEY '');");
    //
    auto result = con.Query(sql);

    ASSERT_FALSE(result->HasError()) << "Returned error:\n" << result->GetError();

    std::cout << "Query result: " << result->ToString() << std::endl;
}

} // namespace flockmtl
