#ifndef META_ZIP_WITH_HPP__
#define META_ZIP_WITH_HPP__

template<template<typename, typename> typename F, typename A, typename B>
struct ZipWith;

template<template<typename, typename> typename F, typename A, typename B>
using ZipWithT = typename ZipWith<F, A, B>::Type;

#endif
