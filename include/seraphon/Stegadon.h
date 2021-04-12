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

        Stegadon(WayOfTheSeraphon way, Constellation constellation, WeaponOption option, bool skinkChief, CommandTrait trait, Artefact artefact, bool isGeneral);

        Stegadon() = delete;

        ~Stegadon() override;

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

        Weapon  m_javelins{Weapon::Type::Missile, "Meteoric Javelins", 8, 4, 5, 4, 0, 1},
                m_bow{Weapon::Type::Missile, "Skystreak Bow", 24, 3, 3, 3, -1, 3},
                m_throwers{Weapon::Type::Missile, "Sunfire Throwers", 8, 1, 0, 0, 0, 0},
                m_warspear{Weapon::Type::Melee, "Meteoric Warspear", 1, 3, 3, 3, -1, 1},
                m_horns{Weapon::Type::Melee, "Massive Horns", 2, 2, 3, 3, -1, 4},
                m_jaws{Weapon::Type::Melee, "Grinding Jaws", 1, 2, 3, 3, -1, 2},
                m_stomps{Weapon::Type::Melee, "Crushing Stomps", 1, 5, 3, 3, -1, 2};

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
