/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DAWNRIDERS_H
#define DAWNRIDERS_H

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class Dawnriders : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Dawnriders();

        ~Dawnriders() override = default;

        bool configure(int numModels, bool standardBearer);

    protected:

        void onRestore() override;
        void onStartHero(PlayerId player) override;
        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
        void onCastSpell(const Spell *spell, const Unit *target) override;
        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;
        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
        Rerolls battleshockRerolls() const override;
        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_guardiansSword,
            m_lance,
            m_hooves;

        bool m_standardBearer = false;

        bool m_powerOfHyshActive = false;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Deathly Furrows                  Yes
// Sunmetal Weapons                 Yes
// Lances of Dawn                   Yes
// Power of Hysh                    Yes
//

} // namespace LuminethRealmLords

#endif // DAWNRIDERS_H