#include <gtest/gtest.h>
#include <dmm.h>

size_t memsize = 25;
char *ptr = nullptr;

TEST(test_dmm_avail_before_malloc, test_dmm)
{
    ASSERT_EQ(dmm_avail(), HEAP_SIZE);
}

TEST(test_dmm_malloc_25bytes, test_dmm)
{
    ptr = static_cast<char*>(dmm_malloc(memsize));
    ASSERT_NE(ptr, nullptr);
}

TEST(test_dmm_avail_after_malloc, test_dmm)
{
    ASSERT_LE(dmm_avail(), HEAP_SIZE-memsize);
}

TEST(test_dmm_free, test_dmm)
{
    dmm_free(ptr);
    ASSERT_EQ(dmm_avail(), HEAP_SIZE);
}

TEST(test_dmm_malloc_greater_then_heap_size, test_dmm)
{
    ptr = static_cast<char*>(dmm_malloc(2 * HEAP_SIZE));
    ASSERT_EQ(ptr, nullptr);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
