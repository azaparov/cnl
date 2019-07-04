
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_BINARY_OPERATOR_H)
#define CNL_IMPL_NUMBER_BINARY_OPERATOR_H

#include "../operators/generic.h"
#include "../operators/native_tag.h"
#include "../operators/overloads.h"
#include "../type_traits/enable_if.h"
#include "is_number.h"
#include "make_number.h"
#include "operator_helpers.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    // higher OP number<>
    template<class Operator, class Lhs, class Rhs>
    struct binary_operator<
            _impl::native_tag, Operator, Lhs, Rhs,
            _impl::enable_if_t<std::is_floating_point<Lhs>::value && _impl::is_number<Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(Operator()(lhs, static_cast<Lhs>(rhs)))
        {
            return Operator()(lhs, static_cast<Lhs>(rhs));
        }
    };

    // number<> OP higher
    template<class Operator, class Lhs, class Rhs>
    struct binary_operator<
            _impl::native_tag, Operator, Lhs, Rhs,
            _impl::enable_if_t<_impl::is_number<Lhs>::value && std::is_floating_point<Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(Operator()(static_cast<Rhs>(lhs), rhs))
        {
            return Operator()(static_cast<Rhs>(lhs), rhs);
        }
    };

    // lower OP number<>
    template<class Operator, class Lhs, class Rhs>
    struct binary_operator<
            _impl::native_tag, Operator, Lhs, Rhs,
            _impl::enable_if_t<_impl::number_can_wrap<Rhs, Lhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(Operator()(_impl::from_value<Rhs>(lhs), rhs))
        {
            return Operator()(_impl::from_value<Rhs>(lhs), rhs);
        }
    };

    // number<> OP lower
    template<class Operator, class Lhs, class Rhs>
    struct binary_operator<
            _impl::native_tag, Operator, Lhs, Rhs,
            _impl::enable_if_t<_impl::number_can_wrap<Lhs, Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(Operator()(lhs, _impl::from_value<Lhs>(rhs)))
        {
            return Operator()(lhs, _impl::from_value<Lhs>(rhs));
        }
    };

    template<class Operator, typename LhsRep, typename RhsRep>
    struct binary_operator<_impl::native_tag, Operator, _impl::number<LhsRep>, _impl::number<RhsRep>> {
        CNL_NODISCARD constexpr auto operator()(
                _impl::number<LhsRep> const& lhs, _impl::number<RhsRep> const& rhs) const
        -> decltype(_impl::make_number(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs))))
        {
            return _impl::make_number(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)));
        }
    };
}

#endif  // CNL_IMPL_NUMBER_BINARY_OPERATOR_H