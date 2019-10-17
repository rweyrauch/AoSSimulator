/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ORRUKS_H
#define ORRUKS_H

#include <Unit.h>
#include <Weapon.h>

namespace Greenskinz
{

class Orruks : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    enum WeaponOption
    {
        ChoppaAndShield,
        SpearAndShield,
        PairedChoppas,
        OrrukBowAndCutta
    };

    enum StandardBearer
    {
        None,
        OrrukBanner,
        SkullIcon
    };
    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);

    Orruks();
    ~Orruks() override = default;

    bool configure(int numModels, WeaponOption weapons, bool drummer, StandardBearer standardBearer);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int chargeModifier() const override;
    int braveryModifier() const override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    Rerolls toSaveRerolls(const Weapon *weapon) const override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;

private:

    bool m_waaaghDrummer = false;
    StandardBearer m_standardBearer = None;
    WeaponOption m_weapons = ChoppaAndShield;

    Weapon m_orrukBows,
        m_choppa,
        m_pigstikkaSpear,
        m_cutta,
        m_orrukBowBoss,
        m_choppaBoss,
        m_pigstikkaSpearBoss,
        m_cuttaBoss;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Drummer                          Yes
// Orruk Banner                     Yes
// Skull Icon                       No
// Choppas                          Yes
// Mob Rule                         Yes
// Waaagh! Shield                   Yes
// Ready Boyz! Air! Fire!           Yes
//

} // namespace Greenskinz

#endif // ORRUKS_H