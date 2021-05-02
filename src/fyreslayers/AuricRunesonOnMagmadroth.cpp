/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRunesonOnMagmadroth.h>
#include <Board.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int g_basesize = 120; // x92 oval
    static const int g_pointsPerUnit = 230;

    bool AuricRunesonOnMagmadroth::s_registered = false;

    AuricRunesonOnMagmadroth::AuricRunesonOnMagmadroth() :
            Magmadroth("Auric Runeson on Magmadroth", 8, g_pointsPerUnit),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_javelin(Weapon::Type::Missile, "Wyrmslayer Javelin", 12, 1, 4, 3, -1, RAND_D3),
            m_warAxe(Weapon::Type::Melee, "Ancestral War-axe", 1, 3, 3, 4, 0, RAND_D3),
            m_javelinMelee(Weapon::Type::Melee, "Wyrmslayer Javelin", 3, 1, 3, 3, -1, 1) {
        m_keywords = {ORDER, DUARDIN, MAGMADROTH, FYRESLAYERS, MONSTER, HERO, AURIC_RUNEFATHER};
        m_weapons = {&m_throwingAxe, &m_fyrestream, &m_clawsAndHorns, &m_blazingMaw, &m_javelin, &m_warAxe,
                     &m_javelinMelee};
        m_battleFieldRole = Role::Leader_Behemoth;
    }

    void AuricRunesonOnMagmadroth::configure(MountTrait trait) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMissileWeapon(&m_fyrestream);
        model->addMeleeWeapon(&m_clawsAndHorns);
        model->addMeleeWeapon(&m_blazingMaw);
        model->addMeleeWeapon(&m_javelin);
        model->addMeleeWeapon(&m_warAxe);
        model->addMeleeWeapon(&m_javelinMelee);
        addModel(model);

        m_mountTrait = trait;

        m_points = g_pointsPerUnit;
    }

    Unit *AuricRunesonOnMagmadroth::Create(const ParameterList &parameters) {
        auto unit = new AuricRunesonOnMagmadroth();

        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        unit->setLodge(lodge);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_fatherSonTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_heirloomArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto mount = (MountTrait) GetEnumParam("Mount Trait", parameters, g_mountTraits[0]);

        unit->configure(mount);
        return unit;
    }

    void AuricRunesonOnMagmadroth::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AuricRunesonOnMagmadroth::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    AuricRunesonOnMagmadroth::ComputePoints,
                    {
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                            EnumParameter("Artefact", g_heirloomArtefacts[0], g_heirloomArtefacts),
                            EnumParameter("Command Trait", g_fatherSonTraits[0], g_fatherSonTraits),
                            EnumParameter("Mount Trait", g_mountTraits[0], g_mountTraits),
                            BoolParameter("General")
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Auric Runeson on Magmadroth", factoryMethod);
        }
    }

    Rerolls AuricRunesonOnMagmadroth::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Vying for Glory
        if (Board::Instance()->getUnitWithKeyword(this, owningPlayer(), AURIC_RUNESON, 6.0)) {
            return Rerolls::Failed;
        }
        return Fyreslayer::toHitRerolls(weapon, target);
    }

    Wounds
    AuricRunesonOnMagmadroth::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Wyrmslayer Javelins
        if ((weapon->name() == m_javelin.name()) && target->hasKeyword(MONSTER)) {
            return {weapon->damage() + 2, 0};
        }
        return Fyreslayer::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int AuricRunesonOnMagmadroth::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Fyreslayers