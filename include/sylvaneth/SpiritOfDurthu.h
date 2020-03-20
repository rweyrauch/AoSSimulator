/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPIRITOFDURTHU_H
#define SPIRITOFDURTHU_H

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth {

    class SpiritOfDurthu : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SpiritOfDurthu();

        ~SpiritOfDurthu() override;

        bool configure();

    protected:

        void onStartCombat(PlayerId id) override;

        void onStartHero(PlayerId id) override;

        void onWounded() override;

        int getDamageTableIndex() const;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int championOfTheEverqueensWill(const Unit *target);

    private:

        Weapon m_verdantBlast,
            m_guardianSword,
            m_massiveImpalingTalons;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Groundshaking Stomp              TODO
// Impale                           Yes
// Spirit Paths                     TODO
// Champions of the Everqueen's Will Yes
// Wrathful Guardian                Yes
//

} // namespace Sylvaneth

#endif //SPIRITOFDURTHU_H