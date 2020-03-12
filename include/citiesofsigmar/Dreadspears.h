/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DREADSPEARS_H
#define DREADSPEARS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class Dreadspears : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    Dreadspears();
    ~Dreadspears() override = default;

    bool configure(int numModels, bool standardBearer, bool hornblower);

protected:

    int runModifier() const override;
    int chargeModifier() const override;
    int braveryModifier() const override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    int weaponRend(const Weapon* weapon, const Unit* target, int hitRoll, int woundRoll) const override;

private:

    bool m_standardBearer = false;
    bool m_hornblower = false;

    Weapon m_spear, m_spearLordling;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Coven Guard                      Yes
// Ranks of Cold Steel              Yes
//

} // namespace CitiesOfSigmar

#endif //DREADSPEARS_H