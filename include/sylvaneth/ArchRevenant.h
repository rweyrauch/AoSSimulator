/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ARCHREVENANT_H
#define ARCHREVENANT_H

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth
{

class ArchRevenant : public SylvanethBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    ArchRevenant();
    ~ArchRevenant() override = default;

    bool configure();

protected:

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toSaveRerolls(const Weapon* weapon) const override;

    // Buff shields when not our combat phase.
    void onStartHero(PlayerId id) override { m_crescentShieldProtection = false; }

    // Buff weapons during our combat phase
    void onStartCombat(PlayerId id) override { m_crescentShieldProtection = true; }

private:

    Weapon m_glaive,
        m_tailPincers;

    bool m_crescentShieldProtection = false;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Crescent Shield                  Yes
// Champion of Kurnoth              No
// Ultimate Sacrifice               No
// Call to Battle                   No
//

} // namespace Sylvaneth

#endif //ARCHREVENANT_H