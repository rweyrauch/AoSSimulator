/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth {

    class Alarielle : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Alarielle(Glade glade, bool isGeneral);

        Alarielle() = delete;

        ~Alarielle() override = default;

    protected:

        void onStartHero(PlayerId player) override;

        void onEndMovement(PlayerId player) override;

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

        void onCharged() override;

    private:

        Weapon  m_spearOfKurnoth{Weapon::Type::Missile, "Spear of Kurnoth", 24, 1, 2, 2, -2, 6},
                m_talonOfDwindling{Weapon::Type::Melee, "Talon of Dwindling", 1, 4, 3, 4, 0, 1},
                m_beetleGreatAntlers{Weapon::Type::Melee, "Great Antlers", 1, 4, 3, 2, -2, 5};

        bool m_usedSoulAmphorae = false;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Talon of the Dwindling           TODO
// Lifebloom                        Yes
// Living Battering Ram             Yes
// Swirling Glowspites              Yes
// Soul Amphorae                    Yes
// Metamorphosis                    Partial/TODO
// Ghyran's Wrath                   Yes
//

} // namespace Sylvaneth
