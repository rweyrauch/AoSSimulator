/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DREADLORDBLACKDRAGON_H
#define DREADLORDBLACKDRAGON_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class DreadlordOnBlackDragon : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 105;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 300;

    enum WeaponOption
    {
        ExileBladeAndShield,
        ExileBladeAndCrossbow,
        LanceAndShield,
        LanceAndCrossbow,
        PairExileBlades
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    DreadlordOnBlackDragon();
    ~DreadlordOnBlackDragon() override = default;

    int move() const override;

    bool configure(WeaponOption weapon);

protected:

    void onWounded() override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override
    {
        if ((m_weaponOption == PairExileBlades) && (weapon->name() == m_blade.name()))
        {
            return RerollFailed;
        }
        return Unit::toHitRerolls(weapon, target);
    }
    int toSaveModifier(const Weapon *weapon) const override
    {
        auto mod = Unit::toSaveModifier(weapon);

        // Tyrant Shield
        if (m_weaponOption == LanceAndShield || m_weaponOption == ExileBladeAndShield) mod++;

        return mod;
    }
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int weaponRend(const Weapon* weapon, const Unit* target, int hitRoll, int woundRoll) const override;

private:

    int getDamageTableIndex() const;

    WeaponOption m_weaponOption = LanceAndShield;

    Weapon m_crossbow,
        m_noxiousBreath,
        m_blade,
        m_lance,
        m_jaws,
        m_claws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Lance of Spite                   Yes
// Noxious Breath                   No
// Paired Exile Blades              Yes
// Tyrant Shield                    Yes
// Do Not Disappoint Me             No
//

} // namespace CitiesOfSigmar

#endif //DREADLORDBLACKDRAGON_H