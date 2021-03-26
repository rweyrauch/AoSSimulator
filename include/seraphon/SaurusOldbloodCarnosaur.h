/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class SaurusOldbloodOnCarnosaur : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SaurusOldbloodOnCarnosaur();

        ~SaurusOldbloodOnCarnosaur() override;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int terror(const Unit *target);

    private:

        Weapon m_gauntlet,
                m_spear,
                m_forelimbs,
                m_jaws;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blazing Sunbolts                 TODO
// Blood Frenzy                     TODO
// Cold Ferocity                    Yes
// Pinned Down                      Yes
// Terror                           Yes
// Wrath of the Seraphon            TODO
//

} // namespace Seraphon
