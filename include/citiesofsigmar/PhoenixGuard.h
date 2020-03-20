/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PHOENIXGUARD_H
#define PHOENIXGUARD_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class PhoenixGuard : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        PhoenixGuard();

        ~PhoenixGuard() override = default;

        bool configure(int numModels, bool standardBearer, bool drummer);

    protected:

        int runModifier() const override;

        int chargeModifier() const override;

        int braveryModifier() const override;

        Wounds applyWoundSave(const Wounds &wounds) override;

        bool battleshockRequired() const override;

    private:

        bool m_standardBearer = false;
        bool m_drummer = false;

        Weapon m_halberd,
                m_halberdKeeper;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Drummer                          Yes
// Emboldened                       Yes
// Witness to Destiny               Yes
//

} // namespace CitiesOfSigmar

#endif //PHOENIXGUARD_H