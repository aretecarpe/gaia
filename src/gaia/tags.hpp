#if !defined GAIA_TAGS_HEADER_INCLUDED
#define GAIA_TAGS_HEADER_INCLUDED
#pragma once

namespace gaia {

struct create_t { };
struct open_t { };
struct open_or_create_t { };
struct existing_t { };
struct truncate_t { };

constexpr create_t create_only { };
constexpr open_t open_only { };
constexpr open_or_create_t open_or_create { };
constexpr existing_t existing { };
constexpr truncate_t truncate { };

} // end namespace gaia

#endif