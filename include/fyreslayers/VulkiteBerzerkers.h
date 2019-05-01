/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VULKITEBERZERKERS_H
#define VULKITEBERZERKERS_H

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers
{

class VulkiteBerzerkers : public Fyreslayer
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = 480;

    enum WeaponOption
    {
        HandaxeAndShield,
        WarpickAndShield,
        PairedHandaxes
    };

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);

    VulkiteBerzerkers();
    ~VulkiteBerzerkers() override = default;

    bool configure(int numModels, WeaponOption weapons, bool hornOfGrimnir);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    WeaponOption m_weapons = HandaxeAndShield;
    bool m_hornOfGrimnir = false;

    Weapon m_handaxe,
        m_handaxeKarl,
        m_warpick,
        m_warpickKarl,
        m_throwingAxe;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Horn of Grimnir                  No
// Berserk Fury                     No
// Fyresteel Handaxes               Yes
// Bladed Slingshield               No
//

} // namespace Fyreslayers

#endif //VULKITEBERZERKERS_H