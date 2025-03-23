#include <abc/reference_counter.h>

#include <gtest/gtest.h>

#include <thread>
#include <vector>

namespace abc {
namespace test {

// Test basic functionality of RefCounter
TEST(RefCounterTest, BasicUsage) {
    // Default constructor should set use_count to 1
    RefCounter ref;
    EXPECT_EQ(ref.use_count(), 1);
    
    // Adding a reference should increment count
    ref.add_ref();
    EXPECT_EQ(ref.use_count(), 2);
    
    // Releasing a reference should decrement count
    ref.rel_ref();
    EXPECT_EQ(ref.use_count(), 1);
}

// Test copy constructor
TEST(RefCounterTest, CopyConstructor) {
    RefCounter ref1;
    EXPECT_EQ(ref1.use_count(), 1);
    
    {
        RefCounter ref2(ref1);
        // Both should point to same implementation with count 2
        EXPECT_EQ(ref1.use_count(), 2);
        EXPECT_EQ(ref2.use_count(), 2);
    }
    
    // After ref2 is destroyed, count should be back to 1
    EXPECT_EQ(ref1.use_count(), 1);
}

// Test copy assignment operator
TEST(RefCounterTest, CopyAssignment) {
    RefCounter ref1;
    RefCounter ref2;
    
    EXPECT_EQ(ref1.use_count(), 1);
    EXPECT_EQ(ref2.use_count(), 1);
    
    ref2 = ref1;
    
    // Both should point to ref1's implementation with count 2
    EXPECT_EQ(ref1.use_count(), 2);
    EXPECT_EQ(ref2.use_count(), 2);
    
    // Self-assignment shouldn't change anything
    ref1 = ref1;
    EXPECT_EQ(ref1.use_count(), 2);
}

// Test move constructor
TEST(RefCounterTest, MoveConstructor) {
    RefCounter ref1;
    EXPECT_EQ(ref1.use_count(), 1);
    
    RefCounter ref2(std::move(ref1));
    
    // ref2 should now have the original ref count, ref1 should be null
    EXPECT_EQ(ref2.use_count(), 1);
    EXPECT_EQ(ref1.use_count(), 0); // Null impl_ returns 0
}

// Test move assignment operator
TEST(RefCounterTest, MoveAssignment) {
    RefCounter ref1;
    RefCounter ref2;
    
    EXPECT_EQ(ref1.use_count(), 1);
    EXPECT_EQ(ref2.use_count(), 1);
    
    ref2 = std::move(ref1);
    
    // ref2 should have the original ref count, ref1 should be null
    EXPECT_EQ(ref2.use_count(), 1);
    EXPECT_EQ(ref1.use_count(), 0); // Null impl_ returns 0
    
    // Self-move-assignment shouldn't crash
    ref2 = std::move(ref2);
    EXPECT_EQ(ref2.use_count(), 1);
}

// Test thread safety with multiple threads
TEST(RefCounterTest, ThreadSafety) {
    const int THREAD_COUNT = 10;
    const int OPERATIONS_PER_THREAD = 1000;
    
    RefCounter ref;
    std::vector<std::thread> threads;
    
    // Create threads that increment and decrement the reference count
    for (int i = 0; i < THREAD_COUNT; ++i) {
        threads.emplace_back([&ref]() {
            for (int j = 0; j < OPERATIONS_PER_THREAD; ++j) {
                ref.add_ref();
            }
        });
    }
    
    // Join all threads
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    
    // Expected count: 1 (initial) + THREAD_COUNT * OPERATIONS_PER_THREAD
    EXPECT_EQ(ref.use_count(), 1 + THREAD_COUNT * OPERATIONS_PER_THREAD);
    
    // Now release the references
    threads.clear();
    for (int i = 0; i < THREAD_COUNT; ++i) {
        threads.emplace_back([&ref]() {
            for (int j = 0; j < OPERATIONS_PER_THREAD; ++j) {
                ref.rel_ref();
            }
        });
    }
    
    // Join all threads
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    
    // Should be back to 1
    EXPECT_EQ(ref.use_count(), 1);
}

// Test edge cases
TEST(RefCounterTest, EdgeCases) {
    // Test with null implementation
    RefCounter ref1;
    RefCounter ref2(std::move(ref1));
    
    // ref1 should have null implementation
    EXPECT_EQ(ref1.use_count(), 0);
    
    // These operations should not crash with null implementation
    ref1.add_ref();
    ref1.rel_ref();
    EXPECT_EQ(ref1.use_count(), 0);
    
    // Move from null to existing
    ref2 = std::move(ref1);
    EXPECT_EQ(ref2.use_count(), 0);
}

} // namespace test
} // namespace abc
