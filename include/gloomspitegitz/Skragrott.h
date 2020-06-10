/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKRAGROTT_H
#define SKRAGROTT_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>
#include <spells/GloomspiteSpells.h>

namespace GloomspiteGitz {

    class Skragrott : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        Skragrott();

        ~Skragrott() override = default;

        bool configure(Lore lore);

    protected:

        int castingModifier() const override;

        int unbindingModifier() const override;

        Wounds applyWoundSave(const Wounds &wounds) override;

        void onStartHero(PlayerId playerId) override;

    private:

        Weapon m_daMoonOnnaStikkMissile,
                m_daMoonOnnaStikk;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Babbling Wand                    Yes
// Da Moon Onna Stikk               TODO
// Loonking's Crown                 Yes
// Nikkit! Nikkit!                  Yes
// The Loonking's Entreaty          TODO
//

} // namespace GloomspiteGitz

#endif //SKRAGROTT_H