/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDONDRAK_H
#define LORDONDRAK_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosLordOnKarkadrak : public Unit
{
public:

    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 9;
    static const int POINTS_PER_UNIT = 80;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    ChaosLordOnKarkadrak();
    ~ChaosLordOnKarkadrak() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

private:

    Weapon m_battleAxe,
        m_blade,
        m_hornsAndClaws,
        m_tail;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Brutish Rampage                  No
// Daemonbound                      No
// Fuelled by Carnage               No
// Rune-etched Plating              No
// The Knights of Chaos             No
//

} // SlavesToDarkness

#endif //LORDONDRAK_H