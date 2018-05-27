#ifndef _H_TYPE_VALUE_MAP__
#define _H_TYPE_VALUE_MAP__

#include <utility>
#include <experimental/type_traits>

namespace type_value_map {
    template<typename Key,typename Value>
    struct TypedKey {
        using typed_key_type = TypedKey;
        using value_type = Value;
    };

    namespace detail {
        template<typename Key, auto Val>
        struct KeyValuePair {
            using key = Key;
            static constexpr auto value = Val;
            static KeyValuePair get_pair(Key); //empty declaration bc decltype usage only
        };

        //Type-safe key version
        template<typename Key, typename ValueType, auto val>
        struct KeyValuePair<TypedKey<Key,ValueType>,val> {
            using key = KeyValuePair;
            static constexpr auto value = ValueType(val);
            static KeyValuePair get_pair(Key); //empty declaration bc decltype usage only
        };

        template<typename Key>
        using get_typed_key = typename Key::typed_key_type;
    }

    template<typename Key, auto Val>
    using pair = detail::KeyValuePair<std::experimental::detected_or_t<Key,detail::get_typed_key,Key>,Val>;

    template<typename... Pairs>
    struct map : Pairs... {
        using Pairs::get_pair...;
        template<typename Key>
        using get_pair_impl = decltype(map::get_pair(std::declval<Key>()));

        static constexpr bool empty = sizeof...(Pairs) == 0;
        static constexpr auto size = sizeof...(Pairs);

        template<typename Key>
        static constexpr bool has = std::experimental::is_detected_v<get_pair_impl,Key>;

        template<typename Key>
        static constexpr auto get = get_pair_impl<Key>::value;

        template<template<typename...> typename Apply>
        using pairs = Apply<Pairs...>;

        template<template<typename...> typename Apply>
        using keys = Apply<typename Pairs::key...>;

        template<template<auto...> typename Apply>
        using values = Apply<Pairs::value...>;

        template<typename... newPairs>
        using insert = map<Pairs...,newPairs...>;
    };
}

#endif
