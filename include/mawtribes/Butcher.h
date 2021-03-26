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

    class Butcher : public MawtribesBase {
    public:

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        enum WeaponOption {
            Tenderiser,
            Cleaver
        };

        Butcher() = delete;

        ~Butcher() override;

    protected:

        Butcher(Mawtribe tribe, WeaponOption weaponOption, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

    protected:

        void onCastSpell(const Spell *spell, const Unit *target) override;

        void onUnboundSpell(Unit *caster, int castRoll) override;

        int woundModifier() const override;

    private:

        Weapon m_tenderizer{Weapon::Type::Melee, "Tenderizer", 1, 3, 3, 3, -1, 3},
                m_cleaver{Weapon::Type::Melee, "Cleaver", 1, 3, 3, 3, -2, 2},
                m_bite{Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bloodgruel                       Yes
// Voracious Maw                    Yes
//

} // namespace OgorMawtribes
