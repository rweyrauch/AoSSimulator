/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FREEGENERALGRIFFON_H
#define FREEGENERALGRIFFON_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class FreeguildGeneralOnGriffon : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 105;
    static const int WOUNDS = 13;
    static const int POINTS_PER_UNIT = 320;

    enum WeaponOption
    {
        RuneSword,
        Greathammer,
        Lance
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    FreeguildGeneralOnGriffon();
    ~FreeguildGeneralOnGriffon() override = default;

    int move() const override;

    bool configure(WeaponOption weapon, bool hasShield);

protected:

    int toSaveModifier(const Weapon *weapon) const override;
    int weaponRend(const Weapon* weapon, const Unit* target, int hitRoll, int woundRoll) const override;
    int runModifier() const override;
    int chargeModifier() const override;
    void onWounded() override;

private:

    int getDamageTableIndex() const;

    bool m_shield = false;

    Weapon m_runesword,
        m_greathammer,
        m_lance,
        m_claws,
        m_beak;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Charging Lance                   Yes
// Freeguild Shield                 Yes
// Skilled Rider                    Yes
// Piercing Bloodroar               No
// Rousing Battle Cry               No
//

} // namespace CitiesOfSigmar

#endif //FREEGENERALGRIFFON_H