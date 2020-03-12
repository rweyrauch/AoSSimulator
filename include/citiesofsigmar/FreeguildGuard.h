/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FREEGUARD_H
#define FREEGUARD_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class FreeguildGuard : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    enum WeaponOption
    {
        Halberd,
        Spear,
        Sword
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    FreeguildGuard();
    ~FreeguildGuard() override = default;

    bool configure(int numModels, bool standardBearer, bool drummer, WeaponOption weapons);

protected:

    int runModifier() const override;
    int chargeModifier() const override;
    int braveryModifier() const override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    int toSaveModifier(const Weapon *weapon) const override;

private:

    bool m_standardBearer = false;
    bool m_drummer = false;

    Weapon m_halberd,
        m_spear,
        m_sword,
        m_halberdSergeant,
        m_spearSergeant,
        m_swordSergeant;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Drummer                          Yes
// Standard Bearer                  Yes
// Massed Ranks                     Yes
// Parry and Block                  Yes
// Wall of Spears                   Yes
//

} // namespace CitiesOfSigmar

#endif //FREEGUARD_H