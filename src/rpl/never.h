#pragma once

#include "producer.h"

namespace rpl
{

template <typename Value = empty_value, typename Error = no_error> inline auto never()
{
    return make_producer<Value, Error>([](const auto& consumer) { return lifetime(); });
}

} // namespace rpl
