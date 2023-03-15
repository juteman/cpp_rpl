#pragma once

#include "producer.h"
#include <vector>

namespace rpl
{

template <typename Value, typename Error = no_error> inline auto single(Value&& value)
{
    return make_producer<std::decay_t<Value>, Error>(
        [value = std::forward<Value>(value)](const auto& consumer) mutable {
            consumer.put_next(std::move(value));
            consumer.put_done();
            return lifetime();
        });
}

template <typename Error = no_error> inline auto single()
{
    return make_producer<rpl::empty_value, Error>([](const auto& consumer) {
        consumer.put_next({});
        consumer.put_done();
        return lifetime();
    });
}

template <typename Value, typename Error = no_error> inline auto vector(std::vector<Value>&& values)
{
    return make_producer<Value, Error>([values = std::move(values)](const auto& consumer) mutable {
        for (auto& value : values) { consumer.put_next(std::move(value)); }
        consumer.put_done();
        return lifetime();
    });
}

template <typename Error = no_error> inline auto vector(std::vector<bool>&& values)
{
    return make_producer<bool, Error>([values = std::move(values)](const auto& consumer) {
        for (auto value : values) { consumer.put_next_copy(value); }
        consumer.put_done();
        return lifetime();
    });
}

template <typename Range, typename Value = std::decay_t<decltype(*std::begin(std::declval<Range>()))>>
inline auto range(Range&& range)
{
    return vector(std::vector<Value>(std::begin(range), std::end(range)));
}

inline auto ints(int from, int till)
{
    Expects(from <= till);
    return make_producer<int>([from, till](const auto& consumer) {
        for (auto i = from; i != till; ++i) { consumer.put_next_copy(i); }
        consumer.put_done();
        return lifetime();
    });
}

inline auto ints(int count) { return ints(0, count); }

} // namespace rpl
