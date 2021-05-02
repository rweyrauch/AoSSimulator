/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>
#include <gloomspitegitz/GloomspiteSpells.h>

namespace GloomspiteGitz {

    class Zarbag : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        Zarbag();

        ~Zarbag() override = default;

    protected:

        void configure(Lore lore);

        void onRestore() override {
            GloomspiteGitzBase::onRestore();
            m_usedSnifferSpite = false;
        }

        int castingModifier() const override;

    protected:

        mutable bool m_usedSnifferSpite = false;

    private:

        Weapon m_sickle;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Sniffer Spite                    Yes
// Face of Da Bad Moon              TODO
//

} // namespace GloomspiteGitz
