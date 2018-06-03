# type-map
Type->type and optionally typechecked type->value compile-time maps.

This library aims to give a map-like interface at compile time, with types as keys and compile-time values or types as values.
The map being entirely inside of the type system allows for it to be passed as a normal template parameter(and have the values looked up be constexpr in the case of a type->value map), in contrast to eg. a constexpr tuple with strong types, which you can't easily pass down a call stack of constexpr functions and keep it constexpr.

## Examples
Type->type map example:
```cpp
struct A {};
struct B {};
struct C {};

using type_map::tt_pair,type_map::tt_map;

using my_map = tt_map<tt_pair<A,int>,tt_pair<B,bool>>;

static_assert(my_map::template has<A>);
static_assert(std::is_same_v<typename my_map::template get<B>,bool>);

using my_map_with_c = my_map::template insert<tt_pair<C,double>>;

static_assert(my_map_with_c::size == 3);
```

The type->value map has the possibility of ensuring the value belonging to a key is of the right type:
```cpp
using type_map::tv_pair,type_map::tv_map,type_map::CheckedType

struct A : TypedKey<int> {};
struct B {};

using my_map = tv_map<tv_pair<A,1>,tv_pair<B,false>>;

static_assert(m::template get<A> == 1);
```

## Interface

- empty
  - `true` if the map is empty
- size
  - size of the map
- has<Key>
  - `true` if Key is present in the map
- get<Key>
  - For type->type map:  the type that is associated with Key
  - For type->value map: the value that is associated with Key
- keys<Op>
  - Applies all keys to template<class...>class Op
- values<Op>
  - For type->type map:  applies all values to template<class...>class Op
  - For type->value map: applies all values to template<auto...>class Op
- insert<newPairs...>
  - Adds key,value pairs to the map. Duplicate keys cause undefined behaviour.
