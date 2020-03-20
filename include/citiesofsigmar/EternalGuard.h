/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef COS_ETERNALGUARD_H
#define COS_ETERNALGUARD_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class EternalGuard : public CitizenOfSigmar
{
public:

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static int ComputePoints(int numModels);

    EternalGuard();
    ~EternalGuard() override = default;

    bool configure(int numModels, bool standardBearer, bool hornblower, bool gladeShields);

protected:

    int runModifier() const override;
    int chargeModifier() const override;
    int braveryModifier() const override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    int toWoundModifier(const Weapon *weapon, const Unit *target) const override;
    int toSaveModifier(const Weapon *weapon) const override;

private:

    bool m_hornblower = false,
        m_standardBearer = false,
        m_gladeShields = false;

    Weapon m_spearStave,
        m_spearStaveWarden;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Fortress of Boughs               Yes
// Form Fortress of Boughs          Yes
// Standard Bearer                  Yes
// Hornblower                       Yes
//

} // namespace CitiesOfSigmar

#endif //COS_ETERNALGUARD_H