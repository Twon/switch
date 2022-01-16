#pragma once
/*
namespace Switch { 

namespace detail { 

template<typename T>
constexpr std::size_t extractNumParameters = 0;

template<template<typename> class T, typename... Args>
constexpr std::size_t extractNumParameters<T<Args...>> = sizeof...(Args);


template<typename T>
struct ExtractNumParameters;

template<template<typename> class T, typename... Args>
struct ExtractNumParameters<T<Args...>>
{   
    static constexpr std::size_t value = sizeof...(Args);
};

template<template<typename> class T, typename T1>
struct ExtractNumParameters<T<T1>>
{   
    static constexpr std::size_t value = 1;
};

template<typename T>
struct ExtractNumParameters
{   
    static constexpr std::size_t value = 0;
};

constexpr auto getNthElement(std::size_t index, auto switchCases) 
{
    // Please don't do this in real life, this is for easy of understanding only.  For why see:
    //  -  http://ldionne.com/2015/11/29/efficient-parameter-pack-indexing/
    //  -  http://odinthenerd.blogspot.com/2017/04/tree-based-lookup-why-kvasirmpl-is.html
    constexpr auto getCases = []<template<typename> class T, typename... Cases>(auto index, T<Cases...>)
    {
        using CasesTuple = std::tuple<Cases...>;
        return std::tuple_element_t<index, CasesTuple>{};
    };
    return getCases(index, switchCases);
}

}

}
*/