
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_TYPE_H)
#define CNL_IMPL_NUMBER_TYPE_H

#include "../num_traits/from_value.h"
#include "../operators/generic.h"
#include "../operators/native_tag.h"
#include "../operators/tagged.h"
#include "../type_traits/enable_if.h"
#include "is_number.h"
#include "to_rep.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // a numeric type parameterized on storage and behavior
        template<typename Rep=int, class Tag=native_tag>
        class number {
        public:
            number() = default;

            /// constructor taking an _impl::number type
            template<typename RhsRep>
            // NOLINTNEXTLINE(hicpp-explicit-conversions, google-explicit-constructor)
            constexpr number(number<RhsRep, Tag> const& i)
                    : _rep(convert<Tag, Rep>(to_rep(i)))
            {
            }

            /// constructor taking a number type that isn't _impl::number
            template<class S, enable_if_t<!is_number<S>::value, int> Dummy = 0>
            // NOLINTNEXTLINE(hicpp-explicit-conversions, google-explicit-constructor)
            constexpr number(S const& s)
            : _rep(convert<Tag, Rep>(s))
            {
            }

            template<class S>
            CNL_NODISCARD explicit constexpr operator S() const
            {
                return static_cast<S>(_impl::to_rep(*this));
            }

            template<typename T, class Enable>
            friend struct cnl::to_rep;
        private:
            Rep _rep;
        };
    }
}

#endif  // CNL_IMPL_NUMBER_TYPE_H