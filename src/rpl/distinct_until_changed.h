#pragma once

#include "base/optional.h"
#include "producer.h"

namespace rpl
{
namespace details
{

class distinct_until_changed_helper
{
public:
    template <typename Value, typename Error, typename Generator>
    auto operator()(producer<Value, Error, Generator>&& initial) const
    {
        return make_producer<Value, Error>([initial = std::move(initial)](const auto& consumer) mutable {
            auto previous = consumer.template make_state<std::optional<Value>>();
            return std::move(initial).start(
                [consumer, previous](auto&& value) {
                    if (!(*previous) || (**previous) != value)
                    {
                        *previous = value;
                        consumer.put_next_forward(std::forward<decltype(value)>(value));
                    }
                },
                [consumer](auto&& error) { consumer.put_error_forward(std::forward<decltype(error)>(error)); },
                [consumer] { consumer.put_done(); });
        });
    }
};

} // namespace details

inline auto distinct_until_changed() -> details::distinct_until_changed_helper
{
    return details::distinct_until_changed_helper();
}

} // namespace rpl
