// Copyright 2023 Leonhard S.

#pragma once

// Helper macro for locally disabling C4251 warnings, e.g. for STL containers or pimpl classes.
// Usage:
// #include <vector>
// #include "dbg_census/common/census_macros.h"
//
// class FOO_API Foo {
// ...
// private:
//     class FooImpl;
// DISABLE_WARNING_4251
//     std::unique_ptr<class FooImpl> m_impl;
// ENABLE_WARNING_4251
// };
#ifdef _WIN32
#  define DISABLE_WARNING_4251 __pragma(warning(push)) __pragma(warning(disable: 4251)) \
static_assert(true, "")
#  define ENABLE_WARNING_4251 __pragma(warning(pop)) \
static_assert(true, "")
#else
#  define DISABLE_WARNING_4251 \
static_assert(true, "")
#  define ENABLE_WARNING_4251 \
static_assert(true, "")
#endif
