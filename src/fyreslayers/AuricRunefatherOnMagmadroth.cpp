/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRunefatherOnMagmadroth.h>
#include <Board.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int g_basesize = 120; // x92 oval
    static const int g_pointsPerUnit = 270;

    bool AuricRunefatherOnMagmadroth::s_registered = false;

    AuricRunefatherOnMagmadroth::AuricRunefatherOnMagmadroth() :
            Magmadroth("Auric Runefather on Magmadroth", 8, g_pointsPerUnit),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_grandAxe(Weapon::Type::Melee, "Latchkey Grandaxe", 3, 3, 3, 3, -1, 3) {
        m_keywords = {ORDER, DUARDIN, MAGMADROTH, FYRESLAYERS, MONSTER, HERO, AURIC_RUNEFATHER};
        m_weapons = {&m_throwingAxe, &m_fyrestream, &m_clawsAndHorns, &m_blazingMaw, &m_grandAxe};
        m_battleFieldRole = Role::Leader_Behemoth;
    }

    void AuricRunefatherOnMagmadroth::configure(MountTrait trait) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMissileWeapon(&m_fyrestream);
        model->addMeleeWeapon(&m_clawsAndHorns);
        model->addMeleeWeapon(&m_blazingMaw);
        model->addMeleeWeapon(&m_grandAxe);
        addModel(model);

        m_mountTrait = trait;

        m_points = g_pointsPerUnit;
    }

    Unit *AuricRunefatherOnMagmadroth::Create(const ParameterList &parameters) {
        auto unit = new AuricRunefatherOnMagmadroth();

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

    void AuricRunefatherOnMagmadroth::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AuricRunefatherOnMagmadroth::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    AuricRunefatherOnMagmadroth::ComputePoints,
                    {
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                            EnumParameter("Mount Trait", g_mountTraits[0], g_mountTraits),
                            EnumParameter("Command Trait", g_fatherSonTraits[0], g_fatherSonTraits),
                            EnumParameter("Artefact", g_heirloomArtefacts[0], g_heirloomArtefacts),
                            BoolParameter("General")
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Auric Runefather on Magmadroth", factoryMethod);
        }
    }

    void AuricRunefatherOnMagmadroth::onEndCombat(PlayerId player) {
        Magmadroth::onEndCombat(player);

        // Weapon-breaker
        auto unit = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), HERO, 3.0);
        if (unit) {
            if (Dice::RollD6() == 6) {
                // TODO: buff only affects a single weapon
                unit->buffModifier(Attribute::To_Hit_Melee, -1,
                                   {GamePhase::Battleshock, std::numeric_limits<int>::max(), owningPlayer()});
            }
        }
    }

    int AuricRunefatherOnMagmadroth::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void AuricRunefatherOnMagmadroth::onStartHero(PlayerId player) {
        Magmadroth::onStartHero(player);

        if (owningPlayer() == player) {
            // Stare Down
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) <= 3.0)) {
                unit->buffModifier(Attribute::Bravery, -Dice::RollD3(),
                                   {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
            }
        }
    }

} // namespace Fyreslayers