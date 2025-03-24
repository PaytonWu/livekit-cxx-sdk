// Copyright(c) 2025 - present, Payton Wu (payton.wu@outlook.com) & the contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <abc/reference_counter.h>

namespace abc
{

RefCounterImpl::RefCounterImpl() noexcept : use_count_{ 1 }
{
}

auto RefCounterImpl::increment() noexcept -> void
{
    // Use memory_order_relaxed for increment as this doesn't need to synchronize with other operations
    use_count_.fetch_add(1, std::memory_order_relaxed);
}

auto RefCounterImpl::decrement() noexcept -> bool
{
    // Use memory_order_acq_rel for decrement to ensure proper synchronization
    // when the counter reaches zero
    return use_count_.fetch_sub(1, std::memory_order_acq_rel) == 1;
}

auto RefCounterImpl::use_count() const noexcept -> int
{
    // Use memory_order_acquire to ensure visibility of previous operations
    return use_count_.load(std::memory_order_acquire);
}

RefCounter::RefCounter() : impl_{ new RefCounterImpl() }
{
}

RefCounter::RefCounter(abc::RefCounter const & other) : impl_{ other.impl_ }
{
    if (impl_)
    {
        impl_->increment();
    }
}

auto RefCounter::operator=(RefCounter const & other) noexcept -> RefCounter &
{
    if (this != &other)
    {
        rel_ref();

        impl_ = other.impl_;
        if (impl_)
        {
            impl_->increment();
        }
    }
    return *this;
}

RefCounter::RefCounter(RefCounter && other) noexcept : impl_{nullptr}
{
    impl_ = other.impl_;
    other.impl_ = nullptr;
}

auto RefCounter::operator=(RefCounter && other) noexcept -> RefCounter &
{
    if (this != &other)
    {
        rel_ref();

        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

RefCounter::~RefCounter() noexcept
{
    rel_ref();
}

auto RefCounter::add_ref() -> void
{
    if (impl_)
    {
        impl_->increment();
    }
}

auto RefCounter::rel_ref() -> void
{
    if (impl_ && impl_->decrement())
    {
        delete impl_;
        impl_ = nullptr;
    }
}

auto RefCounter::use_count() const -> int
{
    if (impl_)
    {
        return impl_->use_count();
    }
    return 0;
}

} // namespace abc
