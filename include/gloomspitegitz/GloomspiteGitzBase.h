/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef GLOOMSPITEBASE_H
#define GLOOMSPITEBASE_H

#include <Unit.h>
#include <gloomspitegitz/BadMoon.h>

namespace GloomspiteGitz {

    class GloomspiteGitzBase : public Unit {
    public:
        GloomspiteGitzBase() = default;

        ~GloomspiteGitzBase() override = default;

    protected:

        GloomspiteGitzBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        void onBeginRound(int battleRound) override;

        void onEndRound(int battleRound) override;

        void onStartHero(PlayerId player) override;

        int castingModifier() const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        void onRan() override;

    protected:

        bool inLightOfTheBadMoon() const;

        bool m_movedMoon = false;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fangz of the Bad Moon            Yes
// Bad Moon Magic                   Partial/TODO
// Loonatic Inspiration             TODO
// Lunar Squigs                     Yes
// Moonclan Fungus Brew             Yes
// Spiderfang Venom                 Yes
// Troggoth Renawal                 Yes
//

    void Init();

} // namespace GloomspiteGitz

#endif //GLOOMSPITEBASE_H