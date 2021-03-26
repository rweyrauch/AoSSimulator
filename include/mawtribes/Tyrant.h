/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes {

    class Tyrant : public MawtribesBase {
    public:

        enum class BigName {
            None,
            Deathcheater,
            Brawlerguts,
            Fateseeker,
            Longstrider,
            Giantbreaker,
            Wallcrusher
        };

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Tyrant() = delete;

        ~Tyrant() override;

    protected:

        Tyrant(Mawtribe tribe, BigName bigName, CommandTrait trait, Artefact artefact, bool isGeneral);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        void onStartHero(PlayerId player) override;

        int woundModifier() const override;

        int mightyBellower(const Unit *unit, int roll);

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        BigName m_bigName = BigName::Deathcheater;
        BigName m_bigNameExtra = BigName::None;

        Weapon m_pistols{Weapon::Type::Missile, "Ogor Pistols", 12, 2, 4, 3, -1, RAND_D3},
                m_thundermace{Weapon::Type::Melee, "Thundermace", 1, 3, 3, 3, -2, 3},
                m_glaive{Weapon::Type::Melee, "Beastskewer Glaive", 3, 2, 3, 3, -1, RAND_D3},
                m_bite{Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1};

        lsignal::slot m_mightyBellower;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Beastskewer Glaive               Yes
// Big Name
//        Deathcheater              Yes
//        Brawlerguts               Yes
//        Fateseeker                Yes
//        Longstrider               Yes
//        Giantbreaker              TODO
//        Wallcrusher               TODO
// Thundermace                      Yes
// Bully of the First Degree        TODO
//

} // namespace OgorMawtribes
