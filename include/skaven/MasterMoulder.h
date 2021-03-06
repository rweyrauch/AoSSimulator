/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class MasterMoulder : public Skaventide {
    public:

        enum WeaponOption {
            Lash,
            Things_Catcher
        };

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static void Init();

        MasterMoulder(WeaponOption option, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~MasterMoulder() override;

    protected:

        void onStartHero(PlayerId player) override;

        int crackTheWhip(const Unit *attacker, const Weapon *weapon, const Unit *target);

        int crackTheWhipBravery(const Unit *unit);

    private:

        Weapon m_lash,
                m_catcher;

        lsignal::slot m_whipSlot, m_whipBraverySlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Master Moulder                   Yes
// Crack the Whip                   Yes
// Unleash More-more Beasts!        TODO
//

} // namespace Skaven
