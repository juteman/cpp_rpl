#pragma once


#include <functional>


#ifndef Unexpected
#    define Unexpected(message) std::abort()
#    define UniqueFunctionUnexpected
#endif // Unexpected

namespace base
{
namespace details
{

template <typename Callable> class moveable_callable_wrap
{
public:
    static_assert(std::is_move_constructible_v<Callable>, "Should be at least moveable.");

    moveable_callable_wrap(Callable&& other) : _value(std::move(other)) {}
    moveable_callable_wrap& operator=(Callable&& other)
    {
        _value = std::move(other);
        return *this;
    }
    moveable_callable_wrap(moveable_callable_wrap&& other) : _value(std::move(other._value)) {}
    moveable_callable_wrap(const moveable_callable_wrap& other) : _value(fail_construct()) {}
    moveable_callable_wrap& operator=(moveable_callable_wrap&& other)
    {
        _value = std::move(other._value);
        return *this;
    }
    moveable_callable_wrap& operator=(const moveable_callable_wrap& other) { return fail_assign(); }

    template <typename... Args> decltype(auto) operator()(Args&&... args)
    {
        return _value(std::forward<Args>(args)...);
    }

private:
    [[noreturn]] Callable                fail_construct() { Unexpected("Attempt to copy-construct a move-only type."); }
    [[noreturn]] moveable_callable_wrap& fail_assign() { Unexpected("Attempt to copy-assign a move-only type."); }

    Callable _value;
};

} // namespace details

template <typename Function> class unique_function;

template <typename Return, typename... Args> class unique_function<Return(Args...)> final
{
public:
    unique_function(std::nullptr_t = nullptr) noexcept {}
    unique_function(const unique_function& other)            = delete;
    unique_function& operator=(const unique_function& other) = delete;

    // Move construct / assign from the same type.
    unique_function(unique_function&& other) : _impl(std::move(other._impl)) {}
    unique_function& operator=(unique_function&& other)
    {
        _impl = std::move(other._impl);
        return *this;
    }

    template <typename Callable,
              typename = std::enable_if_t<
                  std::is_convertible_v<decltype(std::declval<Callable>()(std::declval<Args>()...)), Return>>>
    unique_function(Callable&& other) :
        unique_function(std::forward<Callable>(other), std::is_copy_constructible<std::decay_t<Callable>>{})
    {}

    template <typename Callable,
              typename = std::enable_if_t<
                  std::is_convertible_v<decltype(std::declval<Callable>()(std::declval<Args>()...)), Return>>>
    unique_function& operator=(Callable&& other)
    {
        using Decayed = std::decay_t<Callable>;
        if constexpr (std::is_copy_constructible_v<Decayed>) { _impl = std::forward<Callable>(other); }
        else if constexpr (std::is_move_constructible_v<Decayed>)
        {
            _impl = details::moveable_callable_wrap<Decayed>(std::forward<Callable>(other));
        }
        else { static_assert(false_t(other), "Should be moveable."); }
        return *this;
    }

    void swap(unique_function& other) { _impl.swap(other._impl); }

    Return operator()(Args... args) { return _impl(std::forward<Args>(args)...); }

    explicit operator bool() const { return _impl.operator bool(); }

    friend inline bool operator==(const unique_function& value, std::nullptr_t) noexcept
    {
        return value._impl == nullptr;
    }
    friend inline bool operator==(std::nullptr_t, const unique_function& value) noexcept
    {
        return value._impl == nullptr;
    }
    friend inline bool operator!=(const unique_function& value, std::nullptr_t) noexcept
    {
        return value._impl != nullptr;
    }
    friend inline bool operator!=(std::nullptr_t, const unique_function& value) noexcept
    {
        return value._impl != nullptr;
    }

private:
    template <typename Callable> unique_function(Callable&& other, std::true_type) :
        _impl(std::forward<Callable>(other))
    {}

    template <typename Callable> unique_function(Callable&& other, std::false_type) :
        _impl(details::moveable_callable_wrap<std::decay_t<Callable>>(std::forward<Callable>(other)))
    {}

    std::function<Return(Args...)> _impl;
};

} // namespace base

#ifdef UniqueFunctionUnexpected
#    undef UniqueFunctionUnexpected
#    undef Unexpected
#endif // UniqueFunctionUnexpectedb
