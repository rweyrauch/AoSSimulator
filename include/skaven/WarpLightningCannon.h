/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARPLIGHTNINGCANNON_H
#define WARPLIGHTNINGCANNON_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class WarpLightningCannon : public Skaventide
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 180;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    WarpLightningCannon();
    ~WarpLightningCannon() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int generateMortalWounds(const Unit *unit) override;

private:

    Weapon m_warpLightningBlast,
        m_teethAndKnives;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Warp Lightning Blast             Yes
// More-more Warp Lightning         Yes
//

} // namespace Skaven

#endif //WARPLIGHTNINGCANNON_H