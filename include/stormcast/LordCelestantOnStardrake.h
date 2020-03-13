/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDCELESTANTONSTARDRAKE_H
#define LORDCELESTANTONSTARDRAKE_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class LordCelestantOnStardrake : public StormcastEternal
{
public:

    static const int BASESIZE = 170; // x105 oval
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 500;

    enum WeaponOption
    {
        CelestineHammer,
        StormboundBlade,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    LordCelestantOnStardrake();
    ~LordCelestantOnStardrake() override;

    bool configure(WeaponOption weapons);

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;
    Rerolls toSaveRerolls(const Weapon* weapon) const override;
    Wounds computeReturnedDamage(const Weapon* weapon, int saveRoll) const override;
    void onStartShooting(PlayerId player) override;
    void onStartCombat(PlayerId player) override;
    Wounds onEndCombat(PlayerId player) override;
    void onRestore() override;
    int arcaneLineage(const Unit* target);

protected:

    WeaponOption m_weaponOption = CelestineHammer;

private:

    Weapon m_celestineHammer,
        m_stormboundBlade,
        m_greatClaws;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Lineage                   Yes
// Cavernous Jaws                   Yes
// Inescapable Vengeance            Yes
// Lord of the Heavens              Yes
// Sigmarite Thundershield          Yes
// Stormbound Blade                 Yes
// Sweeping Tail                    Yes
// Lord of the Celestial Host       TODO
//

} // namespace StormcastEternals

#endif //LORDCELESTANTONSTARDRAKE_H