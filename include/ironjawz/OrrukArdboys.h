/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <ironjawz/Ironjawz.h>
#include <Weapon.h>

namespace Ironjawz {

    class OrrukArdboys : public Ironjawz {
    public:

        enum StandardOption {
            None = 0,
            BannerBearer,
            GlyphBearer
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        OrrukArdboys(Warclan warclan, int numModels,  int numShields, bool drummer, StandardOption standard, int points);

        ~OrrukArdboys() override;

    protected:

        void computeBattleshockEffect(int roll, int &numFled, int &numAdded) const override;

        int chargeModifier() const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        int braveryModifier() const override;

        int glyphBearer(const Unit *target);

    protected:

        Weapon m_choppa,
                m_bossChoppa;

        int m_numShields = 0;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Drummer                          Yes
// Banner                           Yes
// Glyph Bearer                     Yes
// Orruk-forged Shields             Yes
//

} // namespace Ironjawz
