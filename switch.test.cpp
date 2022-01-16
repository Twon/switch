#define CATCH_CONFIG_MAIN
#include "switch.hpp"
#include <catch2/catch.hpp>

#include <array>
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

TEST_CASE("Support functional switches with sequential enums", "[switch.sequential_enums]")
{
    constexpr std::array colours = { Colours::Red, Colours::Green, Colours::Blue };
    using Cases =   Switch::CaseList<
                        Switch::SwitchCase<Colours, Colours::Red>, 
                        Switch::SwitchCase<Colours, Colours::Green>, 
                        Switch::SwitchCase<Colours, Colours::Blue>
                    >;


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
    }
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
