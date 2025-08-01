// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <livekit/utils/event_loop.h>

#include <gtest/gtest.h>

#include <exec/static_thread_pool.hpp>
#include <stdexec/execution.hpp>

#include <atomic>
#include <chrono>
#include <memory>
#include <thread>
#include <vector>

namespace ex = stdexec;

namespace livekit::utils::test
{

class EventLoopTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Set up a thread pool for testing async operations
        thread_pool = std::make_unique<exec::static_thread_pool>(4);
    }

    void TearDown() override
    {
        thread_pool.reset();
    }

    std::unique_ptr<exec::static_thread_pool> thread_pool;
};

// Test TimerHandle basic functionality
TEST_F(EventLoopTest, TimerHandleBasicFunctionality)
{
    TimerHandle handle;

    // Initially in cancelled state
    EXPECT_FALSE(handle.is_cancelled());

    // Cancel and check state
    handle.cancel();
    EXPECT_TRUE(handle.is_cancelled());

    // Multiple cancels should be safe
    handle.cancel();
    EXPECT_TRUE(handle.is_cancelled());
}

// Test TimerHandle thread safety
TEST_F(EventLoopTest, TimerHandleThreadSafety)
{
    TimerHandle handle{};
    std::atomic<int> cancel_count{ 0 };
    std::atomic<int> check_count{ 0 };

    // Start multiple threads that cancel and check the handle
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i)
    {
        threads.emplace_back([handle, &cancel_count]() mutable {
            handle.cancel();
            cancel_count.fetch_add(1);
        });

        threads.emplace_back([handle, &check_count]() {
            if (handle.is_cancelled())
            {
                check_count.fetch_add(1);
            }
        });
    }

    // Wait for all threads to complete
    for (auto & thread : threads)
    {
        thread.join();
    }

    // Verify final state
    EXPECT_TRUE(handle.is_cancelled());
    EXPECT_EQ(cancel_count.load(), 10);
    // check_count should be <= 10 (depends on timing)
    EXPECT_LE(check_count.load(), 10);
}

// Test basic callback execution
TEST_F(EventLoopTest, CallLaterBasicCallback)
{
    EventLoop event_loop;
    std::atomic<bool> callback_executed{ false };

    // Set up execution context
    auto scheduler = thread_pool->get_scheduler();
    auto work = ex::schedule(scheduler) | ex::then([&]() {
                    auto handle = event_loop.call_later(0.01, [&callback_executed]() { callback_executed.store(true); });
                    EXPECT_FALSE(handle.is_cancelled());

                    // Wait a bit for the callback to execute
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                });

    ex::sync_wait(work);
    EXPECT_TRUE(callback_executed.load());
}

// Test callback with arguments
TEST_F(EventLoopTest, CallLaterWithArguments)
{
    EventLoop event_loop;
    std::atomic<int> result{ 0 };

    auto scheduler = thread_pool->get_scheduler();
    auto work = ex::schedule(scheduler) | ex::then([&]() {
                    auto handle = event_loop.call_later(0.01, [&result](int a, int b, int c) { result.store(a + b + c); }, 10, 20, 30);

                    // Wait for callback execution
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                });

    ex::sync_wait(work);
    EXPECT_EQ(result.load(), 60);
}

// Test callback cancellation
TEST_F(EventLoopTest, CallLaterCancellation)
{
    EventLoop event_loop;
    std::atomic<bool> callback_executed{ false };

    auto scheduler = thread_pool->get_scheduler();
    auto work = ex::schedule(scheduler) | ex::then([&]() {
                    auto handle = event_loop.call_later(0.05, [&callback_executed]() { callback_executed.store(true); });

                    EXPECT_FALSE(handle.is_cancelled());

                    // Cancel immediately
                    handle.cancel();
                    EXPECT_TRUE(handle.is_cancelled());

                    // Wait longer than the delay to ensure callback would have fired
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                });

    ex::sync_wait(work);
    EXPECT_FALSE(callback_executed.load());
}

// Test timing accuracy (within reasonable bounds)
TEST_F(EventLoopTest, CallLaterTimingAccuracy)
{
    EventLoop event_loop;
    std::atomic<std::chrono::steady_clock::time_point> execution_time;

    auto scheduler = thread_pool->get_scheduler();
    auto work = ex::schedule(scheduler) | ex::then([&]() {
                    auto start_time = std::chrono::steady_clock::now();
                    double delay_seconds = 0.05; // 50ms

                    auto handle = event_loop.call_later(delay_seconds, [&execution_time]() { execution_time.store(std::chrono::steady_clock::now()); });

                    // Wait for execution
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));

                    auto actual_delay = execution_time.load() - start_time;
                    auto expected_delay = std::chrono::duration<double>(delay_seconds);
                    auto tolerance = std::chrono::milliseconds(20); // 20ms tolerance

                    // Check that the actual delay is within reasonable bounds
                    EXPECT_GE(actual_delay, expected_delay - tolerance);
                    EXPECT_LE(actual_delay, expected_delay + tolerance);
                });

    ex::sync_wait(work);
}

// Test multiple simultaneous timers
TEST_F(EventLoopTest, MultipleConcurrentTimers)
{
    EventLoop event_loop;
    std::atomic<int> execution_count{ 0 };

    auto scheduler = thread_pool->get_scheduler();
    auto work = ex::schedule(scheduler) | ex::then([&]() {
                    std::vector<TimerHandle> handles;

                    // Create multiple timers with different delays
                    for (int i = 0; i < 5; ++i)
                    {
                        double delay = 0.01 * (i + 1); // 10ms, 20ms, 30ms, 40ms, 50ms
                        auto handle = event_loop.call_later(delay, [&execution_count]() { execution_count.fetch_add(1); });
                        handles.push_back(handle);
                    }

                    // All handles should be valid and not cancelled
                    for (auto const & handle : handles)
                    {
                        EXPECT_FALSE(handle.is_cancelled());
                    }

                    // Wait for all timers to execute
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                });

    ex::sync_wait(work);
    EXPECT_EQ(execution_count.load(), 5);
}

// Test timer with zero delay
TEST_F(EventLoopTest, ZeroDelayTimer)
{
    EventLoop event_loop;
    std::atomic<bool> callback_executed{ false };

    auto scheduler = thread_pool->get_scheduler();
    auto work = ex::schedule(scheduler) | ex::then([&]() {
                    auto handle = event_loop.call_later(0.0, [&callback_executed]() { callback_executed.store(true); });

                    // Give it a moment to execute
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                });

    ex::sync_wait(work);
    EXPECT_TRUE(callback_executed.load());
}

// Test callback with captured variables
TEST_F(EventLoopTest, CallbackWithCaptures)
{
    EventLoop event_loop;
    auto result = std::make_shared<std::string>();
    std::atomic<bool> callback_executed{ false };

    auto scheduler = thread_pool->get_scheduler();
    auto work = ex::schedule(scheduler) | ex::then([&]() {
                    std::string captured_value = "test_string";

                    auto handle = event_loop.call_later(0.01, [captured_value, result, &callback_executed]() mutable {
                        *result = captured_value + "_modified";
                        callback_executed.store(true);
                    });

                    // Wait for execution
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                });

    ex::sync_wait(work);

    EXPECT_TRUE(callback_executed.load());
    EXPECT_EQ(*result, "test_string_modified");
}

// Test exception safety in callback
// TEST_F(EventLoopTest, CallbackExceptionSafety)
// {
//     EventLoop event_loop;
//     std::atomic<bool> second_callback_executed{ false };

//     auto scheduler = thread_pool->get_scheduler();
//     auto work = ex::schedule(scheduler) | ex::then([&]() {
//                     // First timer that throws
//                     auto handle1 = event_loop.call_later(0.01, []() { throw std::runtime_error("Test exception"); });

//                     // Second timer that should still execute
//                     auto handle2 = event_loop.call_later(0.02, [&second_callback_executed]() { second_callback_executed.store(true); });

//                     // Wait for both to execute
//                     std::this_thread::sleep_for(std::chrono::milliseconds(50));
//                 });

//     // This should not throw despite the exception in the first callback
//     EXPECT_NO_THROW(ex::sync_wait(work));

//     // The second callback should still execute
//     EXPECT_TRUE(second_callback_executed.load());
// }

// Test handle lifetime after EventLoop destruction
TEST_F(EventLoopTest, HandleLifetimeAfterEventLoopDestruction)
{
    TimerHandle handle;

    {
        EventLoop event_loop;
        handle = event_loop.call_later(10.0, []() {
            // This should not execute since we'll cancel
        });

        EXPECT_FALSE(handle.is_cancelled());
    } // EventLoop destroyed here

    // Handle should still be valid and we should be able to cancel it
    EXPECT_NO_THROW(handle.cancel());
    EXPECT_TRUE(handle.is_cancelled());
}

} // namespace livekit::utils::test
