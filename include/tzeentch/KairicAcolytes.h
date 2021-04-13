/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class KairicAcolytes : public TzeentchBase {
    public:

        enum WeaponOptions {
            Cursed_Blade,
            Paired_Cursed_Blades,
            Cursed_Blade_And_Shield
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        KairicAcolytes();

        ~KairicAcolytes() override = default;

        bool configure(int numModels, WeaponOptions weapons, int numCursedGlaives, int numScrollsOfDarkArts,
                       int numVulcharcs);

        void activateGestaltSorcery() { m_gestaltSorceryActive = true; }

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        int castingModifier() const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target,
                       int hitRoll, int woundRoll) const override;

        void onRestore() override;

        void onStartHero(PlayerId player) override { m_gestaltSorceryActive = false; }

    private:

        bool m_gestaltSorceryActive = false;

        WeaponOptions m_weaponOption = Cursed_Blade;
        int m_numCursedGlaives = 0;
        int m_numScrollsOfDarkArts = 0;
        int m_numVulcharcs = 0;

        Weapon m_sorcerousBolt,
                m_cursedBlade,
                m_cursedGlaive,
                m_cursedBladeAdept,
                m_cursedGlaiveAdept;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Arcanite Shields                 Yes
// Gestalt Sorcery                  Yes
// Paired Cursed Blades             Yes
// Scroll of Dark Arts              Yes
// Vulcharc                         TODO
//

} // Tzeentch
