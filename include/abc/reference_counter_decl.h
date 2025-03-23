#ifndef LIVEKIT_CXX_SDK_INCLUDE_ABC_REFERENCE_COUNTER_DECL
#define LIVEKIT_CXX_SDK_INCLUDE_ABC_REFERENCE_COUNTER_DECL

#pragma once

#include "reference_counter_fwd_decl.h"

#include <atomic>

namespace abc
{

class RefCounterImpl final
{
private:
    std::atomic<int> use_count_;

public:
    RefCounterImpl() noexcept;

    RefCounterImpl(RefCounterImpl const &) = delete;
    auto operator=(RefCounterImpl const &) -> RefCounterImpl & = delete;

    // Atomically increment the reference count
    // Thread-safe operation
    auto increment() noexcept -> void;

    // Atomically decrement the reference count
    // Returns true if the reference count is zero after decrement
    // Thread-safe operation
    auto decrement() noexcept -> bool;

    // Get the current reference count (for debugging/testing)
    // Note: This is a snapshot and may be immediately outdated in a multi-threaded context
    auto use_count() const noexcept -> int;
};

class RefCounter final
{
private:
    RefCounterImpl * impl_;

public:
    RefCounter();
    RefCounter(RefCounter const & other);
    auto operator=(RefCounter const & other) noexcept -> RefCounter &;
    RefCounter(RefCounter && other) noexcept;
    auto operator=(RefCounter && other) noexcept -> RefCounter &;
    ~RefCounter() noexcept;

    // explicit add reference and release reference
    auto add_ref() -> void;
    auto rel_ref() -> void;

    auto use_count() const -> int;
};

} // namespace abc

#endif // LIVEKIT_CXX_SDK_INCLUDE_ABC_REFERENCE_COUNTER_DECL
