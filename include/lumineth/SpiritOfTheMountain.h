/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPIRITOFTHEMOUNTAIN_H
#define SPIRITOFTHEMOUNTAIN_H

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class AlarithSpiritOfTheMountain : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AlarithSpiritOfTheMountain();

        ~AlarithSpiritOfTheMountain() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

    private:

        int getDamageTableIndex() const;

        Weapon m_blast,
            m_hammer,
            m_hooves;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// All but Immovable                TODO
// Ponderous Advance                TODO
// Stoneheart Shockwave             TODO
// Stonemage Symbiosis              TODO
// Faith of the Mountain            TODO
//

} // namespace LuminethRealmLords

#endif // SPIRITOFTHEMOUNTAIN_H
