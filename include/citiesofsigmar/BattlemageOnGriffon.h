/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BATTLEMAGEGRIFFON_H
#define BATTLEMAGEGRIFFON_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class BattlemageOnGriffon : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        BattlemageOnGriffon();

        ~BattlemageOnGriffon() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        int castingModifier() const override;

    private:

        int getDamageTableIndex() const;

        Weapon m_beastStaff,
            m_twinBeaks,
            m_razorClaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Amber Battlemage                 Yes
// Two-headed                       Yes
// Amber Spear                      TODO
// Wildform                         TODO
//

} // namespace CitiesOfSigmar

#endif //BATTLEMAGEGRIFFON_H