#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>

#include <fmt/format.h>

#include <stdexec/execution.hpp>

// Minimal unbounded MPSC channel implemented with stdexec senders
// - UnboundedSender<T>::send(T) pushes values
// - UnboundedReceiver<T>::recv() returns a sender that completes with std::optional<T>
//   - std::optional<T>{T} when a value is available
//   - std::nullopt when the sending side is closed and no more values

// Forward declarations
template <typename T>
class UnboundedSender;

template <typename T>
class UnboundedReceiver;

// Shared state
template <typename T>
class ChannelState
{
public:
    std::queue<T> queue;
    std::mutex mutex;

    bool sender_disconnected = false;
    bool receiver_disconnected = false;

    // A single waiter for simplicity (fits example needs). Could be extended to a list.
    std::optional<std::function<void(std::optional<T>)>> waiting_callback;

    friend class UnboundedSender<T>;
    friend class UnboundedReceiver<T>;
};

// Sender returned by UnboundedReceiver::recv()
template <typename T>
struct RecvSender
{
    using sender_concept = stdexec::sender_t;
    using completion_signatures = stdexec::completion_signatures<stdexec::set_value_t(std::optional<T>)>;

    std::shared_ptr<ChannelState<T>> state;
};

// Provide get_completion_signatures via tag_invoke for ADL
template <typename T, class Env>
auto tag_invoke(stdexec::get_completion_signatures_t, RecvSender<T> const &, Env &&) -> stdexec::completion_signatures<stdexec::set_value_t(std::optional<T>)>
{
    return {};
}

// Operation state for RecvSender
template <typename T, typename Receiver>
struct RecvOperation
{
    std::shared_ptr<ChannelState<T>> state;
    Receiver rcvr;

    friend void start(RecvOperation & op)
    {
        std::optional<std::optional<T>> immediate_result;
        {
            std::lock_guard<std::mutex> lock(op.state->mutex);
            // Always check queue first - even if sender is disconnected, there might be queued values
            if (!op.state->queue.empty())
            {
                T value = std::move(op.state->queue.front());
                op.state->queue.pop();
                immediate_result.emplace(std::move(value));
            }
            else if (op.state->sender_disconnected)
            {
                // Only return nullopt if queue is empty AND sender is disconnected
                immediate_result.emplace(std::nullopt);
            }
            else
            {
                // Install a callback to complete when a value arrives or sender closes
                // The callback will be invoked from send() or destructor, and must signal run_loop
                op.state->waiting_callback = [rcvr = std::move(op.rcvr)](std::optional<T> v) mutable {
                    // set_value will call the receiver's set_value, which calls run_loop.finish()
                    // This wakes up run_loop.run() which is waiting in sync_wait
                    stdexec::set_value(std::move(rcvr), std::move(v));
                };
            }
        }

        if (immediate_result)
        {
            stdexec::set_value(std::move(op.rcvr), std::move(*immediate_result));
        }
    }
};

// start CPO customization via tag_invoke
template <typename T, typename Receiver>
auto tag_invoke(stdexec::start_t, RecvOperation<T, Receiver> & op) noexcept -> void
{
    start(op);
}

// connect CPO customization via tag_invoke
template <typename T, typename Receiver>
auto tag_invoke(stdexec::connect_t, RecvSender<T> const & sender, Receiver && rcvr)
{
    return RecvOperation<T, std::remove_cvref_t<Receiver>>{ sender.state, std::forward<Receiver>(rcvr) };
}

// UnboundedSender
template <typename T>
class UnboundedSender
{
private:
    std::shared_ptr<ChannelState<T>> state;

public:
    explicit UnboundedSender(std::shared_ptr<ChannelState<T>> s) : state(std::move(s))
    {
    }

    // Move constructor: shared_ptr move already nulls the source
    UnboundedSender(UnboundedSender && other) noexcept : state(std::move(other.state))
    {
        // Ensure moved-from object has null state to prevent destructor from setting sender_disconnected
        other.state = nullptr;
    }

    // Move assignment
    UnboundedSender & operator=(UnboundedSender && other) noexcept
    {
        if (this != &other)
        {
            state = std::move(other.state);
            // Ensure moved-from object has null state
            other.state = nullptr;
        }
        return *this;
    }

    // Delete copy to prevent accidental copies
    UnboundedSender(UnboundedSender const &) = delete;
    UnboundedSender & operator=(UnboundedSender const &) = delete;

    bool send(T value)
    {
        if (!state)
        {
            return false;
        }

        std::optional<std::function<void(std::optional<T>)>> to_notify;
        {
            std::lock_guard<std::mutex> lock(state->mutex);
            if (state->receiver_disconnected)
            {
                return false;
            }
            if (state->waiting_callback)
            {
                to_notify = std::move(state->waiting_callback);
                state->waiting_callback.reset();
            }
            else
            {
                state->queue.push(std::move(value));
            }
        }

        if (to_notify)
        {
            (*to_notify)(std::optional<T>{ std::move(value) });
        }
        return true;
    }

    [[nodiscard]] bool is_closed() const
    {
        if (!state)
        {
            return true;
        }
        std::lock_guard<std::mutex> lock(state->mutex);
        return state->receiver_disconnected;
    }

    ~UnboundedSender()
    {
        if (state)
        {
            std::optional<std::function<void(std::optional<T>)>> to_notify;
            {
                std::lock_guard<std::mutex> lock(state->mutex);
                state->sender_disconnected = true;
                if (state->waiting_callback)
                {
                    to_notify = std::move(state->waiting_callback);
                    state->waiting_callback.reset();
                }
            }
            if (to_notify)
            {
                (*to_notify)(std::nullopt);
            }
        }
    }
};

// UnboundedReceiver
template <typename T>
class UnboundedReceiver
{
private:
    std::shared_ptr<ChannelState<T>> state;

public:
    explicit UnboundedReceiver(std::shared_ptr<ChannelState<T>> s) : state(std::move(s))
    {
    }

    // Move constructor - ensure moved-from receiver doesn't mark as disconnected
    UnboundedReceiver(UnboundedReceiver && other) noexcept : state(std::move(other.state))
    {
        other.state = nullptr;
    }

    // Move assignment
    UnboundedReceiver & operator=(UnboundedReceiver && other) noexcept
    {
        if (this != &other)
        {
            state = std::move(other.state);
            other.state = nullptr;
        }
        return *this;
    }

    // Delete copy
    UnboundedReceiver(UnboundedReceiver const &) = delete;
    UnboundedReceiver & operator=(UnboundedReceiver const &) = delete;

    RecvSender<T> recv()
    {
        return RecvSender<T>{ state };
    }

    ~UnboundedReceiver()
    {
        if (state)
        {
            std::lock_guard<std::mutex> lock(state->mutex);
            state->receiver_disconnected = true;
        }
    }
};

// Factory
template <typename T>
std::pair<UnboundedSender<T>, UnboundedReceiver<T>> channel()
{
    auto s = std::make_shared<ChannelState<T>>();
    return { UnboundedSender<T>{ s }, UnboundedReceiver<T>{ s } };
}

int main()
{
    std::cout << "[main] Creating channel and starting threads...\n";
    auto channel_pair = channel<int>();
    auto tx = std::move(channel_pair.first);
    auto rx = std::move(channel_pair.second);

    // Explicitly move tx to ensure proper move semantics
    UnboundedSender<int> producer_tx = std::move(tx);

    std::jthread producer([tx = std::move(producer_tx)]() mutable {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        fmt::print("[sender] send 10\n");
        tx.send(10);

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        fmt::print("[sender] send 20\n");
        tx.send(20);

        fmt::print("[sender] done\n");
    });

    // Consume until channel closes, using stdexec::sync_wait on the recv sender
    for (;;)
    {
        auto got = stdexec::sync_wait(rx.recv());
        if (!got.has_value())
        {
            fmt::print("[receiver] closed (no tuple)\n");
            break;
        }
        // sync_wait returns std::optional<std::tuple<std::optional<T>>> by default
        // Extract the std::optional<T>
        auto opt = std::get<0>(*got);
        if (!opt)
        {
            fmt::print("[receiver] closed\n");
            break;
        }
        fmt::print("[receiver] got {}\n", *opt);
    }

    fmt::print("[main] finished\n");
    return 0;
}
