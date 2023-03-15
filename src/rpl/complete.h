#pragma once

#include "producer.h"

namespace rpl
{

template <typename Value = empty_value, typename Error = no_error> inline auto complete()
{
    return make_producer<Value, Error>([](const auto& consumer) {
        consumer.put_done();
        return lifetime();
    });
}

} // namespace rpl
