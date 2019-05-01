/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLADEBRINGERSEEKERCHARIOT_H
#define BLADEBRINGERSEEKERCHARIOT_H

#include <Unit.h>
#include <Weapon.h>

namespace Slaanesh
{

class BladebringerOnSeekerChariot : public Unit
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 160;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    BladebringerOnSeekerChariot();
    ~BladebringerOnSeekerChariot() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onCharged() override;

private:

    Weapon m_flensingWhips,
        m_piercingClaws,
        m_poisonedTongues;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Impossibly Swift                 Yes
// Multilating Blades               Yes
// Acquiescence                     No
//

} // Slannesh

#endif //BLADEBRINGERSEEKERCHARIOT_H