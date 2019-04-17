/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DRAKESWORNTEMPLAR_H
#define DRAKESWORNTEMPLAR_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class DrakeswornTemplar : public StormcastEternal
{
public:

    static const int BASESIZE = 170; // x105 oval
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 560;

    enum WeaponOption
    {
        TempestAxe,
        ArcHammer,
        Stormlance,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    DrakeswornTemplar();
    ~DrakeswornTemplar() override = default;

    bool configure(WeaponOption weapons, bool skyboltBow);
    int move() const override;

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

    void onStartShooting(PlayerId player) override;
    void onStartCombat(PlayerId player) override;
    void onEndCombat(PlayerId player) override;
    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

protected:

    WeaponOption m_weapons = TempestAxe;

private:

    Weapon m_skyboltBow,
        m_tempestAxe,
        m_arcHammer,
        m_stormlance,
        m_greatClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Arcane Lineage                   No
// Cavernous Jaws                   Yes
// Sweeping Tail                    Yes
// Arc Hammer                       Yes
// Lord of the Heavens              No
// Skybolt Bow                      No
// Stormlance                       Yes
// Tempest Axe                      No
//

} // namespace StormcastEternals

#endif //DRAKESWORNTEMPLAR_H