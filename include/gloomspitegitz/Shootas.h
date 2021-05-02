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

    class Shootas : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Shootas(int points);

        ~Shootas() override = default;

        bool configure(int numModels, int numBarbedNets,
                       int numGongbashers, int numFlagbearers, int numIconbearers);

    protected:

        int toWoundModifier(const Weapon *weapon, const Unit *unit) const override;

        int toHitModifier(const Weapon *weapon, const Unit *unit) const override;

        int runModifier() const override;

        int braveryModifier() const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_slitta,
                m_slittaBoss,
                m_moonclanBow,
                m_moonclanBowBoss,
                m_barbedNet;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Backstabbing Mob                 Yes
// Moonclan Bows                    Yes
// Netters                          Yes
//

} // namespace GloomspiteGitz
