#pragma once

#include "producer.h"

namespace rpl
{
namespace details
{

class type_erased_helper
{
public:
    template <typename Value, typename Error, typename Generator>
    producer<Value, Error> operator()(producer<Value, Error, Generator>&& initial) const
    {
        return std::move(initial);
    }
};

} // namespace details

inline auto type_erased() -> details::type_erased_helper { return details::type_erased_helper(); }

} // namespace rpl
