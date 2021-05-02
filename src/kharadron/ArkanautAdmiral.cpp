/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/ArkanautAdmiral.h>
#include <UnitFactory.h>
#include "KharadronPrivate.h"

namespace KharadronOverlords {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 120;

    bool ArkanautAdmiral::s_registered = false;

    Unit *ArkanautAdmiral::Create(const ParameterList &parameters) {
        auto unit = new ArkanautAdmiral();

        auto port = (Skyport) GetEnumParam("Skyport", parameters, g_skyport[0]);
        unit->setSkyport(port);

        auto artycle = (Artycle) GetEnumParam("Artycle", parameters, g_artycles[0]);
        auto amendment = (Amendment) GetEnumParam("Amendment", parameters, g_amendments[0]);
        auto footnote = (Footnote) GetEnumParam("Footnote", parameters, g_footnotes[0]);
        unit->setCode(artycle, amendment, footnote);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_admiralCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_admiralArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
        return unit;
    }

    void ArkanautAdmiral::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ArkanautAdmiral::Create,
                    KharadronBase::ValueToString,
                    KharadronBase::EnumStringToInt,
                    ArkanautAdmiral::ComputePoints,
                    {
                            EnumParameter("Skyport", g_skyport[0], g_skyport),
                            EnumParameter("Artycle", g_artycles[0], g_artycles),
                            EnumParameter("Amendment", g_amendments[0], g_amendments),
                            EnumParameter("Footnote", g_footnotes[0], g_footnotes),
                            EnumParameter("Command Trait", g_admiralCommandTraits[0], g_admiralCommandTraits),
                            EnumParameter("Artefact", g_admiralArtefacts[0], g_admiralArtefacts),
                            BoolParameter("General")
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Arkanaut Admiral", factoryMethod);
        }
    }

    ArkanautAdmiral::ArkanautAdmiral() :
            KharadronBase("Arkanaut Admiral", 4, g_wounds, 8, 3, false, g_pointsPerUnit),
            m_pistol(Weapon::Type::Missile, "Volley Pistol", 9, 3, 3, 4, -1, 1),
            m_skalfhammer(Weapon::Type::Melee, "Skalfhammer", 1, 3, 3, 2, -2, 2) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, HERO, SKYFARER, MARINE, ARKANAUT_ADMIRAL};
        m_weapons = {&m_pistol, &m_skalfhammer};
        m_battleFieldRole = Role::Leader;
    }

    void ArkanautAdmiral::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_pistol);
        model->addMeleeWeapon(&m_skalfhammer);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Rerolls ArkanautAdmiral::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // If You Want A Job Done...
        if (!weapon->isMissile() && (target->hasKeyword(HERO) || target->hasKeyword(MONSTER))) return Rerolls::Ones;

        return KharadronBase::toHitRerolls(weapon, target);
    }

    Rerolls ArkanautAdmiral::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // If You Want A Job Done...
        if (!weapon->isMissile() && (target->hasKeyword(HERO) || target->hasKeyword(MONSTER))) return Rerolls::Ones;

        return KharadronBase::toWoundRerolls(weapon, target);
    }

    int ArkanautAdmiral::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //KharadronOverlords
