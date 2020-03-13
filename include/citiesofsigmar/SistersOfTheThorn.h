/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SISTEROFTHORN_H
#define SISTEROFTHORN_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class SistersOfTheThorn : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 130;
    static const int POINTS_MAX_UNIT_SIZE = 130*4;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    SistersOfTheThorn();
    ~SistersOfTheThorn() override = default;

    bool configure(int numModels, bool standardBearer, bool hornblower);

protected:

    int runModifier() const override;
    int chargeModifier() const override;
    int braveryModifier() const override;

private:

    bool m_standardBearer = false;
    bool m_hornblower = false;

    Weapon m_javelin,
        m_staff,
        m_antlersAndHooves,
        m_staffMaiden;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Armour of Thorns                 TODO
//

} // namespace CitiesOfSigmar

#endif //SISTEROFTHORN_H