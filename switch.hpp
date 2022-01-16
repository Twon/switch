#pragma once

#include <boost/preprocessor/repetition.hpp>
#include <boost/hana.hpp>
#include <boost/mp11.hpp>
#include <concepts>
#include <tuple>
#include <type_traits>
#include "expand.hpp"

namespace Switch {

template<typename... Cases>
struct CaseList 
{ 
};

constexpr auto switchCaseElement(auto case_, auto f) {return f(case_); }
constexpr auto switchCondition(auto case_) { return switchCaseElement(case_, boost::hana::first); }
constexpr auto switchTag(auto case_) { return switchCaseElement(case_, boost::hana::second); }


#ifndef SWITCH_MAX
#define SWITCH_MAX 64
#endif

inline constexpr auto defaultCase = [](auto unhandledValue){ return unhandledValue; };

// We accept any hana Product (of case × tag), and also single element serving as both case and tag (see below).
template<class CT, CT C, class T = std::integral_constant<CT, C>>
using SwitchCase = boost::hana::pair<std::integral_constant<CT, C>, T>;

namespace detail { 

#define SWITCH_CASE(Z,N,_)      \
    case decltype(switchCondition(boost::mp11::mp_at_c<CaseList, N>{}))::value:                     \
        return std::forward<CaseHandler>(caseHander)(decltype(switchCondition(boost::mp11::mp_at_c<CaseList, N>{})){});


#define SWITCH_OVERLOAD(Z,N,_)                                                                                          \
template<class Result, class Condition, class CaseList, class CaseHandler, class DefaultCase>                           \
constexpr Result switch_(                                                                                               \
    std::integral_constant<std::size_t, N>,                                                                             \
    Condition const condition,                                                                                          \
    CaseList const cases,                                                                                               \
    CaseHandler&& caseHander,                                                                                           \
    DefaultCase&& defaultCase                                                                                           \
)                                                                                                                       \
{                                                                                                                       \
    switch(condition)                                                                                                   \
    {                                                                                                                   \
        BOOST_PP_REPEAT_ ## Z(N, SWITCH_CASE, ~)                                                                        \
    }                                                                                                                   \
    return defaultCase(condition);                                                                                      \
}

BOOST_PP_REPEAT(SWITCH_MAX, SWITCH_OVERLOAD, ~)
#undef SWITCH_CASE
#undef SWITCH_OVERLOAD

template<class Condition, class CaseList, class CaseHandler, class DefaultHandler>
auto switchResult(CaseList caseList)
{
    constexpr std::size_t Size = boost::mp11::mp_size<CaseList>::value;
    if constexpr (Size == 0u)
        return std::invoke_result_t<DefaultHandler, Condition>{};
    else
        return [&]<std::size_t... I>(std::index_sequence<I...>)
        {
            return std::common_type_t<
                std::invoke_result_t<CaseHandler, decltype(switchTag(boost::mp11::mp_at_c<CaseList, I>{}))>...>{};
        }(std::make_index_sequence<Size>{});
}

template<class Condition, class CaseList, class CaseHandler, class DefaultHandler>
using SwitchResult = decltype(switchResult<Condition, CaseList, CaseHandler, DefaultHandler>(std::declval<CaseList>()));

}

template<class Condition, class CaseList, class CaseHandler, class DefaultCase = decltype(defaultCase)>
constexpr auto switch_(
    Condition const condition, /// The switch conditional value to be executed.
    CaseList const caseList, /// List of case predicates.
    CaseHandler&& caseHander, /// Handler for case statement bodies. 
    DefaultCase&& defaultCase = DefaultCase{} /// Handler for the default statement of the switch. 
)
{
    constexpr std::size_t Size = boost::mp11::mp_size<CaseList>::value;
    static_assert(Size < SWITCH_MAX, "Increse SWITCH_MAX");
    using Result = detail::SwitchResult<Condition, CaseList, CaseHandler, DefaultCase>;
    return detail::switch_<Result, Condition, CaseList, CaseHandler, DefaultCase>
        (std::integral_constant<std::size_t, Size>{}, condition, caseList, 
        std::forward<CaseHandler>(caseHander), 
        std::forward<DefaultCase>(defaultCase)
    );
}

}