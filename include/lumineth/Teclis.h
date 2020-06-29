/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TECLIS_H
#define TECLIS_H

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class ArchmageTeclis : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ArchmageTeclis();

        ~ArchmageTeclis() override;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        int auraOfCelennar(const Unit *caster);

    private:

        int getDamageTableIndex() const;

        Weapon m_staff,
            m_sword,
            m_talons;

        lsignal::slot m_auraConnection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Archmage                         TODO
// Aura of Celennar                 Yes
// Discs of the Aelementiri         TODO
// Seeing Stone of Celennar         TODO
// Protection of Teclis             TODO
// Storm of Searing White Light     TODO
//

} // namespace LuminethRealmLords

#endif // TECLIS_H
