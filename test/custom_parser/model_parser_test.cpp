// #include "flockmtl/custom_parser/query/model_parser.hpp"
// #include "flockmtl/custom_parser/tokenizer.hpp"
// #include "gtest/gtest.h"
// #include <memory>
//
// using namespace flockmtl;
//
// /**************************************************
//  *                 Create Model                  *
//  **************************************************/
//
// TEST(ModelParserTest, ParseCreateModel) {
//     std::unique_ptr<QueryStatement> statement;
//     ModelParser parser;
//     EXPECT_NO_THROW(parser.Parse("CREATE MODEL ('test_model', 'model_data', 'provider', {\"context_window\": 512,
//     \"max_output_tokens\": 128})", statement)); ASSERT_NE(statement, nullptr); auto create_stmt =
//     dynamic_cast<CreateModelStatement*>(statement.get()); ASSERT_NE(create_stmt, nullptr);
//     EXPECT_EQ(create_stmt->model_name, "test_model");
//     EXPECT_EQ(create_stmt->model, "model_data");
//     EXPECT_EQ(create_stmt->provider_name, "provider");
//     EXPECT_EQ(create_stmt->model_args["context_window"], 512);
//     EXPECT_EQ(create_stmt->model_args["max_output_tokens"], 128);
// }
//
// TEST(ModelParserTest, ParseCreateGlobalModel) {
//     std::unique_ptr<QueryStatement> statement;
//     ModelParser parser;
//     EXPECT_NO_THROW(parser.Parse("CREATE GLOBAL MODEL ('test_model', 'model_data', 'provider', {\"context_window\":
//     512, \"max_output_tokens\": 128})", statement)); ASSERT_NE(statement, nullptr); auto create_stmt =
//     dynamic_cast<CreateModelStatement*>(statement.get()); ASSERT_NE(create_stmt, nullptr);
//     EXPECT_EQ(create_stmt->model_name, "test_model");
//     EXPECT_EQ(create_stmt->model, "model_data");
//     EXPECT_EQ(create_stmt->provider_name, "provider");
//     EXPECT_EQ(create_stmt->model_args["context_window"], 512);
//     EXPECT_EQ(create_stmt->model_args["max_output_tokens"], 128);
//     EXPECT_EQ(create_stmt->catalog, "flockmtl_storage.");
// }
//
// TEST(ModelParserTest, ParseCreateLocalModel) {
//     std::unique_ptr<QueryStatement> statement;
//     ModelParser parser;
//     EXPECT_NO_THROW(parser.Parse("CREATE LOCAL MODEL ('test_model', 'model_data', 'provider', {\"context_window\":
//     512, \"max_output_tokens\": 128})", statement)); ASSERT_NE(statement, nullptr); auto create_stmt =
//     dynamic_cast<CreateModelStatement*>(statement.get()); ASSERT_NE(create_stmt, nullptr);
//     EXPECT_EQ(create_stmt->model_name, "test_model");
//     EXPECT_EQ(create_stmt->model, "model_data");
//     EXPECT_EQ(create_stmt->provider_name, "provider");
//     EXPECT_EQ(create_stmt->model_args["context_window"], 512);
//     EXPECT_EQ(create_stmt->model_args["max_output_tokens"], 128);
//     EXPECT_EQ(create_stmt->catalog, "");
// }
//
// TEST(ModelParserTest, ParseInvalidCreateModel) {
//     std::unique_ptr<QueryStatement> statement;
//     ModelParser parser;
//     EXPECT_THROW(parser.Parse("CREATE MODEL ('test_model', 'model_data')", statement), std::runtime_error);
// }
//
// /**************************************************
//  *                 Delete Model                  *
//  **************************************************/
//
// TEST(ModelParserTest, ParseDeleteModel) {
//     std::unique_ptr<QueryStatement> statement;
//     ModelParser parser;
//     EXPECT_NO_THROW(parser.Parse("DELETE MODEL 'test_model';", statement));
//     ASSERT_NE(statement, nullptr);
//     const auto delete_stmt = dynamic_cast<DeleteModelStatement*>(statement.get());
//     ASSERT_NE(delete_stmt, nullptr);
//     EXPECT_EQ(delete_stmt->model_name, "test_model");
// }
//
// TEST(ModelParserTest, ParseInvalidDeleteModel) {
//     std::unique_ptr<QueryStatement> statement;
//     ModelParser parser;
//     EXPECT_THROW(parser.Parse("DELETE MODEL", statement), std::runtime_error);
// }
//
// /**************************************************
//  *                 Update Model                  *
//  **************************************************/
//
// TEST(ModelParserTest, ParseUpdateModel) {
//     std::unique_ptr<QueryStatement> statement;
//     ModelParser parser;
//     EXPECT_NO_THROW(parser.Parse("UPDATE MODEL ('test_model', 'new_model_data', 'new_provider', {\"context_window\":
//     1024, \"max_output_tokens\": 256})", statement)); ASSERT_NE(statement, nullptr); const auto update_stmt =
//     dynamic_cast<UpdateModelStatement*>(statement.get()); ASSERT_NE(update_stmt, nullptr);
//     EXPECT_EQ(update_stmt->model_name, "test_model");
//     EXPECT_EQ(update_stmt->new_model, "new_model_data");
//     EXPECT_EQ(update_stmt->provider_name, "new_provider");
//     EXPECT_EQ(update_stmt->new_model_args["context_window"], 1024);
//     EXPECT_EQ(update_stmt->new_model_args["max_output_tokens"], 256);
// }
//
// TEST(ModelParserTest, ParseUpdateModelScopeToGlobal) {
//     std::unique_ptr<QueryStatement> statement;
//     ModelParser parser;
//     EXPECT_NO_THROW(parser.Parse("UPDATE MODEL 'test_model' TO GLOBAL;", statement));
//     ASSERT_NE(statement, nullptr);
//     const auto update_stmt = dynamic_cast<UpdateModelScopeStatement*>(statement.get());
//     ASSERT_NE(update_stmt, nullptr);
//     EXPECT_EQ(update_stmt->model_name, "test_model");
//     EXPECT_EQ(update_stmt->catalog, "flockmtl_storage.");
// }
//
// TEST(ModelParserTest, ParseUpdateModelScopeToLocal) {
//     std::unique_ptr<QueryStatement> statement;
//     ModelParser parser;
//     EXPECT_NO_THROW(parser.Parse("UPDATE MODEL 'test_model' TO LOCAL;", statement));
//     ASSERT_NE(statement, nullptr);
//     const auto update_stmt = dynamic_cast<UpdateModelScopeStatement*>(statement.get());
//     ASSERT_NE(update_stmt, nullptr);
//     EXPECT_EQ(update_stmt->model_name, "test_model");
//     EXPECT_EQ(update_stmt->catalog, "");
// }
//
// TEST(ModelParserTest, ParseInvalidUpdateModel) {
//     std::unique_ptr<QueryStatement> statement;
//     ModelParser parser;
//     EXPECT_THROW(parser.Parse("UPDATE MODEL ('test_model', 'new_model_data')", statement), std::runtime_error);
// }
//
// /**************************************************
//  *                   Get Model                   *
//  **************************************************/
//
// TEST(ModelParserTest, ParseGetModel) {
//     std::unique_ptr<QueryStatement> statement;
//     ModelParser parser;
//     EXPECT_NO_THROW(parser.Parse("GET MODEL 'test_model';", statement));
//     ASSERT_NE(statement, nullptr);
//     auto get_stmt = dynamic_cast<GetModelStatement*>(statement.get());
//     ASSERT_NE(get_stmt, nullptr);
//     EXPECT_EQ(get_stmt->model_name, "test_model");
// }
//
// TEST(ModelParserTest, ParseInvalidGetModel) {
//     std::unique_ptr<QueryStatement> statement;
//     ModelParser parser;
//     EXPECT_THROW(parser.Parse("GET MODEL", statement), std::runtime_error);
// }
//
// /**************************************************
//  *                Get All Models                 *
//  **************************************************/
//
// TEST(ModelParserTest, ParseGetAllModels) {
//     std::unique_ptr<QueryStatement> statement;
//     ModelParser parser;
//     EXPECT_NO_THROW(parser.Parse("GET MODELS;", statement));
//     ASSERT_NE(statement, nullptr);
//     auto get_all_stmt = dynamic_cast<GetAllModelStatement*>(statement.get());
//     ASSERT_NE(get_all_stmt, nullptr);
// }
//
// TEST(ModelParserTest, ParseEmptyQuery) {
//     std::unique_ptr<QueryStatement> statement;
//     ModelParser parser;
//     EXPECT_THROW(parser.Parse("", statement), std::runtime_error);
// }