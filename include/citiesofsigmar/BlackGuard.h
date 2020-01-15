/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLACKGUARD_H
#define BLACKGUARD_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class BlackGuard : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 3;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    BlackGuard();
    ~BlackGuard() override = default;

    bool configure(int numModels, bool standardBearer, bool drummer);

protected:

    int runModifier() const override;
    int chargeModifier() const override;
    int braveryModifier() const override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;

private:

    bool m_standardBearer = false;
    bool m_drummer = false;

    Weapon m_halberd, m_halberdCaptain;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Drummer                          Yes
// Elite Bodyguard                  Yes
//

} // namespace CitiesOfSigmar

#endif //BLACKGUARD_H