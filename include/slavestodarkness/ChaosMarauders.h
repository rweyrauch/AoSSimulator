/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSMARAUDERS_H
#define CHAOSMARAUDERS_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosMarauders : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 20;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 200;

    enum WeaponOption
    {
        Flail = 0,
        Axe,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    ChaosMarauders();
    ~ChaosMarauders() override = default;

    bool configure(int numModels, WeaponOption weapons, bool shields, bool damnedIcon, bool tribalBanner, bool drummer);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onWounded() override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    int runModifier() const override;
    int chargeModifier() const override;
    int braveryModifier() const override;
    void onRestore() override;
    void onStartCombat(PlayerId player) override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

protected:

    bool m_damnedIcon = false;
    bool m_tribalBanner = false;
    bool m_drummer = false;
    bool m_hasShields = false;

    int m_hordeHitModifier = 0;
    int m_hordeWoundModifier = 0;

private:

    Weapon m_axe,
        m_flail,
        m_axeChieftain,
        m_flailChieftain;


    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Damned Icon Bearer               Yes
// Drummer                          Yes
// Darkwood Shields                 Yes
// Mark of Chaos                    No
// Barbarian Hordes                 Yes
// Tribal Banner                    Yes
//

} // SlavesToDarkness

#endif //CHAOSMARAUDERS_H