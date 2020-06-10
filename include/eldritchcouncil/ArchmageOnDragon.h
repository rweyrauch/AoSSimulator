/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ARCHMAGEONDRAGON_H
#define ARCHMAGEONDRAGON_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace EldritchCouncil {

    class ArchmageOnDragon : public Unit {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ArchmageOnDragon();

        ~ArchmageOnDragon() override = default;

        bool configure(bool arcaneTome, bool talisman);

    protected:

        void onWounded() override;

        int unbindingModifier() const override;

        void onRestore() override;

        int getDamageTableIndex() const;

    private:

        bool m_arcaneTome = false;
        bool m_talismanOfArcanePower = false;

        Weapon m_magestaff,
                m_sorcerousBlade,
                m_dragonClaws,
                m_dragonJaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Talisman of Arcane Power         Yes
// Dragonfire                       TODO
// Drain Magic                      TODO
//

} // namespace EldritchCouncil

#endif //ARCHMAGEONDRAGON_H