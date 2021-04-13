/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class VorgarothAndSkalok : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        VorgarothAndSkalok() = delete;

        ~VorgarothAndSkalok() override;

    protected:

        VorgarothAndSkalok(SlaughterHost host, bool isGeneral);

        size_t getDamageTableIndex() const;

        void onWounded() override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onCharged() override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        void onRestore() override;

        int wingsOfFury(const Unit *caster);

        void onStartHero(PlayerId player) override;

    private:

        lsignal::slot m_wingOfFuryConnection;

        Weapon  m_balefire{Weapon::Type::Missile, "White-hot Balefire", 20, 3, 4, 2, -3, RAND_D6},
                m_skullCleaverAxeOfKhorne{Weapon::Type::Melee, "Skull Cleaver Axe of Khorne", 1, 12, 3, 3, -2, 2},
                m_evisceratingClaws{Weapon::Type::Melee, "Eviscerating Claws", 3, RAND_D6, 3, 3, -3, 3},
                m_cavernousJaws{Weapon::Type::Melee, "Cavernous Jaws", 3, 2, 4, 2, -2, 3},
                m_brassPlatedTail{Weapon::Type::Melee, "Brass-plated Tail", 3, RAND_3D6, 4, 4, -1, 3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Monstrous Trophies               Yes
// Crushing Bulk                    Yes
// Eternal Hunt                     TODO
// Fettered Servitude               TODO
// Fuelled by Death                 Yes
// Wings of Fury                    Yes
//

} // namespace Khorne
