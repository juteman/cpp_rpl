#pragma once

#include "filter.h"
#include "producer.h"

namespace rpl
{
template <typename SideEffect> inline auto before_next(SideEffect&& method)
{
    return filter([method = std::forward<SideEffect>(method)](const auto& value) {
        details::callable_invoke(method, value);
        return true;
    });
}

} // namespace rpl
