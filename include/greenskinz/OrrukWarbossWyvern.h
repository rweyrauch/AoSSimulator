/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ORRUKWARBOSSWYVERN_H
#define ORRUKWARBOSSWYVERN_H

#include <Unit.h>
#include <Weapon.h>

namespace Greenskinz {

    class OrrukWarbossOnWyvern : public Unit {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        OrrukWarbossOnWyvern();

        ~OrrukWarbossOnWyvern() override = default;

        bool configure(bool pairedChoppas);

    protected:

        int getDamageTableIndex() const;

        void onWounded() override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Rerolls toSaveRerolls(const Weapon *weapon) const override;

        void onRestore() override;

    private:

        bool m_pairedChoppas = false;

        Weapon m_bossChoppa,
                m_hornsClawsAndTeeth,
                m_barbedTail;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Agonising Venom                  TODO
// Boss Shield                      Yes
// Choppa Boss                      Yes
// Waaagh!                          TODO
//

} // namespace Greenskinz

#endif // ORRUKWARBOSSWYVERN_H