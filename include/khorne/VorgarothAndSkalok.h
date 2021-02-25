/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VORGAROTHANDSKALOK_H
#define VORGAROTHANDSKALOK_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class VorgarothAndSkalok : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        VorgarothAndSkalok();

        ~VorgarothAndSkalok() override;

        bool configure();

    protected:

        int getDamageTableIndex() const;

        void onWounded() override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onCharged() override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        void onRestore() override;

        int wingsOfFury(const Unit *caster);

        void onStartHero(PlayerId player) override;

    private:

        lsignal::slot m_wingOfFuryConnection;

        Weapon m_balefire,
                m_skullCleaverAxeOfKhorne,
                m_evisceratingClaws,
                m_cavernousJaws,
                m_brassPlatedTail;

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

#endif //VORGAROTHANDSKALOK_H