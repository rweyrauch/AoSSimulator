/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LOONBOSSONGIANTCAVESQUIG_H
#define LOONBOSSONGIANTCAVESQUIG_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz {

    class LoonbossOnGiantCaveSquig : public GloomspiteGitzBase {
    public:

        enum WeaponOptions {
            Mooncutta,
            Moonclan_Stabba,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        LoonbossOnGiantCaveSquig();

        ~LoonbossOnGiantCaveSquig() override = default;

        bool configure(WeaponOptions weapon);

    protected:

        void onStartHero(PlayerId player) override;

        int toHitModifier(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        bool m_eatenRedcapMushroom = false;
        Rerolls m_toHitRerolls = Rerolls::None;
        Rerolls m_toWoundRerolls = Rerolls::None;

        Weapon m_massiveFangFilledGob,
                m_moonCutta,
                m_moonclanStabba;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Redcap Mushrooms                 Yes
// Let's Get Bouncing               TODO
//

} // namespace GloomspiteGitz

#endif //LOONBOSSONGIANTCAVESQUIG_H