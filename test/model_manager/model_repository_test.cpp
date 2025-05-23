// #include "flockmtl/model_manager/repository.hpp"
// #include <gtest/gtest.h>
// #include <unordered_map>
//
// namespace flockmtl {
//
// TEST(ModelRepositoryTest, GetProviderType) {
//     EXPECT_EQ(GetProviderType("openai"), FLOCKMTL_OPENAI);
//     EXPECT_EQ(GetProviderType("azure"), FLOCKMTL_AZURE);
//     EXPECT_EQ(GetProviderType("ollama"), FLOCKMTL_OLLAMA);
//     EXPECT_EQ(GetProviderType("default"), FLOCKMTL_OPENAI);
//     EXPECT_EQ(GetProviderType(""), FLOCKMTL_OPENAI);
//     EXPECT_EQ(GetProviderType("unknown"), FLOCKMTL_UNSUPPORTED_PROVIDER);
// }
//
// TEST(ModelRepositoryTest, GetProviderName) {
//     EXPECT_EQ(GetProviderName(FLOCKMTL_OPENAI), OPENAI);
//     EXPECT_EQ(GetProviderName(FLOCKMTL_AZURE), AZURE);
//     EXPECT_EQ(GetProviderName(FLOCKMTL_OLLAMA), OLLAMA);
//     EXPECT_EQ(GetProviderName(FLOCKMTL_UNSUPPORTED_PROVIDER), "");
// }
//
// }// namespace flockmtl