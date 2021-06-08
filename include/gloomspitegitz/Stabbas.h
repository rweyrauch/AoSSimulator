/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz {

    class Stabbas : public GloomspiteGitzBase {
    public:

        enum WeaponOption {
            Stabba,
            Pokin_Spear
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Stabbas(Allegiance allegiance, int numModels, WeaponOption weapons, WeaponOption bossWeapon, int numBarbedNets,
                int numGongbashers, int numFlagbearers, int numIconbearers, int points);

        ~Stabbas() override = default;

    protected:

        int toWoundModifier(const Weapon *weapon, const Unit *unit) const override;

        int runModifier() const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        int braveryModifier() const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_stabba,
                m_stabbaBoss,
                m_pokinSpear,
                m_pokinSpearBoss,
                m_barbedNet;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Backstabbing Mob                 Yes
// Moonshields                      Yes
// Netters                          Yes
//

} // namespace GloomspiteGitz
