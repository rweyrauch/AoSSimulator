/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class Stegadon : public SeraphonBase {
    public:

        enum WeaponOption {
            Skystreak_Bow,
            Sunfire_Throwers
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Stegadon();

        ~Stegadon() override;

        bool configure(WeaponOption option, bool skinkChief);

    protected:

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

        void onCharged() override;

        void onStartCombat(PlayerId player) override;

        int targetSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        Rerolls steadfastMajestyBraveryReroll(const Unit *unit);

    private:

        bool m_skinkChief = false;

        Weapon m_javelins,
                m_bow,
                m_throwers,
                m_warspear,
                m_horns,
                m_jaws,
                m_stomps;

        lsignal::slot m_steadfastSlot;

        Unit *m_armouredCrestAttacker = nullptr;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Armoured Crest                   Yes
// Gout of Sunfire                  TODO
// Steadfast Majesty                Yes
// Unstoppable Stampede             Yes
// Skink Chief                      Yes
// Coordinated Strike               TODO
//

} // namespace Seraphon
