/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <gloomspitegitz/BadMoon.h>
#include <Dice.h>
#include <AgeOfSigmarSim.h>

namespace GloomspiteGitz
{

BadMoon* BadMoon::s_instance = nullptr;

BadMoon* BadMoon::Instance()
{
    if (s_instance == nullptr)
    {
        s_instance = new BadMoon();
    }
    return s_instance;
}

bool BadMoon::setup(Location initialLocation)
{
    if (m_round != 0)
        return false;
    if (initialLocation == Location::Rising || initialLocation == Location::All || initialLocation == Location::Set)
        return false;

    m_initialLocation = initialLocation;
    m_location = Location::Rising;

    return true;
}

bool BadMoon::move(int round)
{
    if (round <= m_round)
    {
        // Already moved
        return false;
    }

    // Bad Moon does not move in the first round
    if (round != 1)
    {
        if (m_location != Location::Set)
        {
            int roll = Dice::rollD6();
            if (roll == 6)
            {
                // advance two positions
                advanceOne();
                advanceOne();
            }
            else if (roll == 1)
            {
                // moon does not move
            }
            else
            {
                // advance one position
                advanceOne();
            }

            SimLog(Verbosity::Narrative, "Bad Moon moves in round %d with a roll of %d.  Location: %d\n",
                round, roll, m_location);
        }
    }
    m_round = round;

    return true;
}

BadMoon::Location BadMoon::location() const
{
    if (m_round > 1)
        return m_location;
    return Location::Rising;
}

void BadMoon::advanceOne()
{
    switch (m_initialLocation)
    {
        case Location::Northwest:
            // Traveling from NW to SE
            if (location() == Location::Rising)
                m_location = Location::Northwest;
            else if (location() == Location::Northwest)
                m_location = Location::All;
            else if (location() == Location::All)
                m_location = Location::Southeast;
            else if (location() == Location::Southeast)
                m_location = Location::Set;
            break;
        case Location::Northeast:
            // Traveling from NE to SW
            if (location() == Location::Rising)
                m_location = Location::Northeast;
            else if (location() == Location::Northeast)
                m_location = Location::All;
            else if (location() == Location::All)
                m_location = Location::Southwest;
            else if (location() == Location::Southwest)
                m_location = Location::Set;
            break;
        case Location::Southeast:
            // Travelling from SE to NW
            if (location() == Location::Rising)
                m_location = Location::Southeast;
            else if (location() == Location::Southeast)
                m_location = Location::All;
            else if (location() == Location::All)
                m_location = Location::Northwest;
            else if (location() == Location::Northwest)
                m_location = Location::Set;
            break;
        case Location::Southwest:
            // Travelling from SW to NE
            if (location() == Location::Rising)
                m_location = Location::Southwest;
            else if (location() == Location::Southwest)
                m_location = Location::All;
            else if (location() == Location::All)
                m_location = Location::Northeast;
            else if (location() == Location::Northeast)
                m_location = Location::Set;
            break;
        default:
            break;
    }
}

} //namespace GloomspiteGitz
