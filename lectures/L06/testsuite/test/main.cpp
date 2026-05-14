/**
 * @brief Test suite using Google Tests.
 */
#include <cstdint>

#include <gtest/gtest.h>

/**
 * @brief Initialize and run test suite with Google Test.
 * 
 * @param[in] argc Number of input arguments when running the test suite.
 * @param[in] argv Vector of input arguments.
 * 
 * @return The number of failed tests.
 */
int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
