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
        struct TagValuePair;

        template<typename TagType, typename ValueType, ValueType val>
        struct TagValuePair<Tag<TagType,ValueType>,val> {
            static constexpr ValueType value = val;
            static TagValuePair get_pair(TagType); //empty declaration bc decltype usage only
        };
    }

    template<typename Tag, auto Val>
    using pair = detail::TagValuePair<typename Tag::tag_type,static_cast<typename Tag::value_type>(Val)>;

    template<typename... Pairs>
    struct map : Pairs... {
        using Pairs::get_pair...;
        template<typename Tag>
        using get_pair_impl = decltype(map::get_pair(std::declval<Tag>()));

        template<typename Tag>
        static constexpr bool has = std::experimental::is_detected_v<get_pair_impl,Tag>;

        template<typename Tag>
        static constexpr auto get = get_pair_impl<Tag>::value;

        template<typename... newPairs>
        using insert = map<Pairs...,newPairs...>;
    };
}
