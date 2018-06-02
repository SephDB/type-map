#ifndef _H_TYPE_VALUE_MAP__
#define _H_TYPE_VALUE_MAP__

#include <utility>
#include <experimental/type_traits>

namespace type_map {
    namespace detail {
        template<typename Key, typename Val>
        struct KeyValuePair {
            using key = Key;
            using value = Val;
            static KeyValuePair get_pair(Key); //empty declaration bc decltype usage only
        };
    }

    template<typename Key, typename Val>
    using tt_pair = detail::KeyValuePair<Key,Val>;

    template<typename... Pairs>
    struct tt_map : Pairs... {
        using Pairs::get_pair...;
        template<typename Key>
        using get_pair_impl = decltype(tt_map::get_pair(std::declval<Key>()));

        static constexpr bool empty = sizeof...(Pairs) == 0;
        static constexpr auto size = sizeof...(Pairs);

        template<typename Key>
        static constexpr bool has = std::experimental::is_detected_v<get_pair_impl,Key>;

        template<typename Key>
        using get = typename get_pair_impl<Key>::value;

        template<template<typename...> typename Apply>
        using keys = Apply<typename Pairs::key...>;

        template<template<class...> typename Apply>
        using values = Apply<typename Pairs::value...>;

        template<typename... newPairs>
        using insert = tt_map<Pairs...,newPairs...>;
    };

    template<typename Value>
    struct TypedKey {
        using typed_key_type = TypedKey;
        using value_type = Value;
    };

    namespace detail {
        template<typename Key, auto Val>
        struct CheckedValue {
            static constexpr auto value = Val;
        };
        template<typename ValType, auto val>
        struct CheckedValue<TypedKey<ValType>,val> {
            static constexpr ValType value = val;
        };

        template<typename Key>
        using get_typed_key = typename Key::typed_key_type;
    }
    template<typename Key, auto Val>
    using tv_pair = tt_pair<Key,detail::CheckedValue<std::experimental::detected_or_t<Key,detail::get_typed_key,Key>,Val>>;

    template<typename... Pairs>
    struct tv_map : tt_map<Pairs...> {
        using parent = tt_map<Pairs...>;
        template<typename Key>
        static constexpr auto get = parent::template get<Key>::value;

        template<template<auto...> typename Apply>
        using values = Apply<Pairs::value::value...>;

        template<typename... newPairs>
        using insert = tv_map<Pairs...,newPairs...>;
    };
}

#endif
