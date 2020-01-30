/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ENGINEOFTHEGODS_H
#define ENGINEOFTHEGODS_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class EngineOfTheGods : public SeraphonBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 0;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    EngineOfTheGods();
    ~EngineOfTheGods() override = default;

    bool configure();

    int move() const override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

    int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_javelins,
        m_horns,
        m_stomps;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Unstoppable Stampede             Yes
// Steadfast Majesty                No
// Cosmic Engine                    No
//

} // namespace Seraphon

#endif //ENGINEOFTHEGODS_H