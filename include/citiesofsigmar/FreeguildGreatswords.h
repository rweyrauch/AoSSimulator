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

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = 420;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    FreeguildGreatswords();
    ~FreeguildGreatswords() override = default;

    bool configure(int numModels, bool standardBearer, bool hornblower);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

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
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Decapitating Strike              Yes
// Oathsworn Honour Guard           Yes
//

} // namespace CitiesOfSigmar

#endif //FREEGREATSWORD_H