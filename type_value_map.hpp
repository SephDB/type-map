#pragma once

#include <utility>
#include <experimental/type_traits>

namespace type_value_map {
    template<typename TagName,typename Value>
    struct Tag {
        using tag_type = Tag;
        using value_type = Value;
    };

    namespace detail {
        template<typename Tag, auto Val>
        struct TagValuePair {
            using key = Tag;
            static constexpr auto value = Val;
            static TagValuePair get_pair(Tag); //empty declaration bc decltype usage only
        };

        //Type-tagged version
        template<typename TagType, typename ValueType, auto val>
        struct TagValuePair<Tag<TagType,ValueType>,val> {
            using key = TagType;
            static constexpr auto value = ValueType(val);
            static TagValuePair get_pair(TagType); //empty declaration bc decltype usage only
        };

        template<typename Tag>
        using get_tag_type = typename Tag::tag_type;
    }

    template<typename Tag, auto Val>
    using pair = detail::TagValuePair<std::experimental::detected_or_t<Tag,detail::get_tag_type,Tag>,Val>;

    template<typename... Pairs>
    struct map : Pairs... {
        using Pairs::get_pair...;
        template<typename Tag>
        using get_pair_impl = decltype(map::get_pair(std::declval<Tag>()));

        template<typename Tag>
        static constexpr bool has = std::experimental::is_detected_v<get_pair_impl,Tag>;

        template<typename Tag>
        static constexpr auto get = get_pair_impl<Tag>::value;

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
