/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FREECROSSBOWMEN_H
#define FREECROSSBOWMEN_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class FreeguildCrossbowmen : public CitizenOfSigmar
{
public:

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    FreeguildCrossbowmen();
    ~FreeguildCrossbowmen() override = default;

    bool configure(int numModels, bool standardBearer, bool piper);

protected:

    int runModifier() const override;
    int chargeModifier() const override;
    int braveryModifier() const override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    bool m_standardBearer = false;
    bool m_piper = false;

    Weapon m_crossbow,
        m_dagger,
        m_crossbowMarksman;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Piper                            Yes
// Reload, Fire!                    Yes
//

} // namespace CitiesOfSigmar

#endif //FREECROSSBOWMEN_H