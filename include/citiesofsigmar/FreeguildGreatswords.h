/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FREEGREATSWORD_H
#define FREEGREATSWORD_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class FreeguildGreatswords : public CitizenOfSigmar
{
public:

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    FreeguildGreatswords();
    ~FreeguildGreatswords() override = default;

    bool configure(int numModels, bool standardBearer, bool hornblower);

protected:

    int runModifier() const override;
    int chargeModifier() const override;
    int braveryModifier() const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;

private:

    bool m_standardBearer = false;
    bool m_hornblower = false;

    Weapon m_zweihander,
        m_zweihanderChampion;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Decapitating Strike              Yes
// Oathsworn Honour Guard           Yes
//

} // namespace CitiesOfSigmar

#endif //FREEGREATSWORD_H