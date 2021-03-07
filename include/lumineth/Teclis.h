/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

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

        bool configure(Lore lore);

        int rollCasting(UnmodifiedCastingRoll& unmodifiedRoll) const override;

        void enableProtectionOfTeclis();

    protected:

        void onWounded() override;

        void onRestore() override;

        int auraOfCelennar(const Unit *caster);

        void onStartHero(PlayerId player) override;

        Wounds protectionAuras(const Wounds& wounds, const Unit* target, const Unit* attacker);

    private:

        int getDamageTableIndex() const;

        bool m_protectionOfTeclisEnabled = false;

        Weapon m_staff,
            m_sword,
            m_talons;

        lsignal::slot m_auraConnection,
            m_protectionConnection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Archmage                         Yes
// Aura of Celennar                 Yes
// Discs of the Aelementiri         TODO
// Seeing Stone of Celennar         Partial
// Protection of Teclis             Yes
// Storm of Searing White Light     Yes
//

} // namespace LuminethRealmLords

