#include "bump.hpp"
#include "./simpletest/simpletest.h"

using namespace std;

char const* groups[] = {
    "Bump",
};

DEFINE_TEST_G(BumpTest1, Bump)
{
    bump<20 * sizeof(int)> bumper;

    int *x = bumper.allocate<int>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");
    
    int *y = bumper.allocate<int>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");
    
    int *z = bumper.allocate<int>(10);
    TEST_MESSAGE(z == nullptr, "Should have failed to allocate!!!!");
}

int main() {
    bool pass = 1;

    for (auto group : groups) {
        pass &= TestFixture::ExecuteTestGroup(group, TestFixture::OutputMode::Verbose);
    }

    return pass ? 0 : 1;
}