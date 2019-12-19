/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LOONBOSSONCAVESQUIG_H
#define LOONBOSSONCAVESQUIG_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class LoonbossOnGiantCaveSquig : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 110;

    enum WeaponOptions
    {
        Mooncutta,
        MoonclanStabba,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    LoonbossOnGiantCaveSquig();
    ~LoonbossOnGiantCaveSquig() override = default;

    bool configure(WeaponOptions weapon);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onStartHero(PlayerId player) override;
    int toHitModifier(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    bool m_eatenRedcapMushroom = false;
    Rerolls m_toHitRerolls = NoRerolls;
    Rerolls m_toWoundRerolls = NoRerolls;

    Weapon m_massiveFangFilledGob,
        m_moonCutta,
        m_moonclanStabba;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Redcap Mushrooms                 Yes
// Let's Get Bouncing               No
//

} // namespace GloomspiteGitz

#endif //LOONBOSSONCAVESQUIG_H