/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODKNIGHTS_H
#define BLOODKNIGHTS_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class BloodKnights : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        BloodKnights();

        ~BloodKnights() override;

        bool configure(int numModels, bool standardBearers, bool hornblowers);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int toSaveModifier(const Weapon *weapon) const override;

        int rollChargeDistance() const override;

        int standardBearerBraveryMod(const Unit* unit);

    private:

        bool m_standardBearers = false;
        bool m_hornblowers = false;

        Weapon m_templarLanceOrBlade,
            m_templarLanceOrBladeKastellan,
            m_hoovesAndTeeth;

        lsignal::slot m_standardSlot;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// The Hunger                       TODO
// Martial Fury                     Yes
// Bloodshields                     Yes
//

} //namespace Death

#endif //BLOODKNIGHTS_H
