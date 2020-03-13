/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KNIGHTOFSHROUDSONSTEED_H
#define KNIGHTOFSHROUDSONSTEED_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt
{

class KnightOfShroudsOnEtherealSteed : public Nighthaunt
{
public:

    static const int BASESIZE = 75; // x42 oval
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    KnightOfShroudsOnEtherealSteed();
    ~KnightOfShroudsOnEtherealSteed() override = default;

    bool configure();

protected:

private:

    Weapon m_sword,
        m_hoovesAndTeeth;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Stolen Hours                     TODO
// Lord of Gheists                  TODO
//


} // namespace Nighthaunt

#endif // KNIGHTOFSHROUDSONSTEED_H
