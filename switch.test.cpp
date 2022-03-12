#define CATCH_CONFIG_MAIN
#include "switch.hpp"
#include <catch2/catch.hpp>

#include <array>
#include <concepts>
#include <cstdint>

enum class Colours : std::uint32_t
{
    Red,
    Green,
    Blue
};

/*
template <typename... T> class Vector;
static_assert(Switch::detail::ExtractNumParameters<Vector<int>>::value == 1, "Increse SWITCH_MAX");
static_assert(Switch::detail::ExtractNumParameters<Vector<int, float>>::value == 2, "Increse SWITCH_MAX");
static_assert(Switch::detail::ExtractNumParameters<Vector<int, int, int>>::value == 3, "Increse SWITCH_MAX");
*/

template<typename Type, class... X>
concept AllOf = (std::same_as<Type, X> && ...);

struct ReturnLValueReference
{
    struct Tag {} b, c, i, f;
    Tag& operator()(bool) { return b; }
    Tag& operator()(char) { return c; }
    Tag& operator()(int) { return i; }
    Tag& operator()(float) { return f; }
};

TEST_CASE("Support functional switches that returns references", "[switch.returns_references]")
{
    using Cases =   Switch::CaseList<
                        Switch::SwitchCase<std::size_t, 0, bool>, 
                        Switch::SwitchCase<std::size_t, 1, char>, 
                        Switch::SwitchCase<std::size_t, 2, int>,
                        Switch::SwitchCase<std::size_t, 3, float>
                    >;

    ReturnLValueReference retRef;
    ReturnLValueReference::Tag& result = Switch::switch_(4, Cases{}, std::ref(retRef));// std::ref(retRef));
    REQUIRE(&result == &retRef.f);
}

struct ReturnRValueReference
{
    struct Tag {} b, c, i, f;
    Tag&& operator()(bool) { return std::move(b); }
    Tag&& operator()(char) { return std::move(c); }
    Tag&& operator()(int) { return std::move(i); }
    Tag&& operator()(float) { return std::move(f); }
};

TEST_CASE("Support functional switches that returns rvalue references", "[switch.returns_rvalue_references]")
{
    using Cases =   Switch::CaseList<
                        Switch::SwitchCase<std::size_t, 0, bool>, 
                        Switch::SwitchCase<std::size_t, 1, char>, 
                        Switch::SwitchCase<std::size_t, 2, int>,
                        Switch::SwitchCase<std::size_t, 3, float>
                    >;

    ReturnRValueReference retRef;
    ReturnRValueReference::Tag&& result = Switch::switch_(4, Cases{}, std::ref(retRef));// std::ref(retRef));
    REQUIRE(&result == &retRef.f);
}

TEST_CASE("Support functional switches with sequential enums", "[switch.sequential_enums]")
{
/*    constexpr std::array colours = { Colours::Red, Colours::Green, Colours::Blue };
    constexpr auto buildSwitchCases = []<typename T, T... t>(std::integral_constant<T, t>... cases) 
    {
        return Switch::CaseList<
                    Switch::SwitchCase<typename decltype(cases)::type, decltype(cases)::value>...
                >{};
    };

    using Cases = decltype(buildSwitchCases.template operator()<Colours>(Colours::Red, Colours::Green, Colours::Blue));


    for (auto colour : colours)
    {
        Switch::switch_(colour, Cases{},
            []<typename Case>(Case caseValue)
            {
                std::cout << "Hello world for case " << static_cast<int>(Case::value) << std::endl;
                return Case::value;
            }
//            []()
//            {
//                std::cout << "Default case" << std::endl;
//            }
        );
    }*/
}


/*

enum MessageTypes
{

};*/

//concept Message = requires(T t){
//    T::Id;
//};


    /*class CaseHandler {
    public:
        constexpr auto operator()(auto)
        {
            REQUIRE(false);
        }

        constexpr auto operator()(int)
        {
            REQUIRE(true);
        }
    };*/


#include <boost/mp11.hpp>


template <typename... T> class Vector;

static_assert(boost::mp11::mp_size<
                Switch::CaseList<
                    Switch::SwitchCase<Colours, Colours::Red>, 
                    Switch::SwitchCase<Colours, Colours::Green>, 
                    Switch::SwitchCase<Colours, Colours::Blue>
                >
            >::value == 3);

static_assert(boost::mp11::mp_size<Vector<int, float, double>>::value == 3);
