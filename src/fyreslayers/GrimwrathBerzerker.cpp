/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/GrimwrathBerzerker.h>
#include <Board.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int g_basesize = 32;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 100;

    bool GrimwrathBerzerker::s_registered = false;

    GrimwrathBerzerker::GrimwrathBerzerker() :
            Fyreslayer("Grimwrath Berzerker", 4, g_wounds, 9, 4, false),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_greatAxe(Weapon::Type::Melee, "Fyrestorm Greataxe", 1, 4, 3, 3, -2, 2) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, GRIMWRATH_BERZERKER};
        m_weapons = {&m_throwingAxe, &m_greatAxe};
    }

    bool GrimwrathBerzerker::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMeleeWeapon(&m_greatAxe);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *GrimwrathBerzerker::Create(const ParameterList &parameters) {
        auto unit = new GrimwrathBerzerker();

        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        unit->setLodge(lodge);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_wrathAndDoomArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GrimwrathBerzerker::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GrimwrathBerzerker::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    GrimwrathBerzerker::ComputePoints,
                    {
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                            EnumParameter("Artefact", g_wrathAndDoomArtefacts[0], g_wrathAndDoomArtefacts),
                            BoolParameter("General")
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Grimwrath Berzerker", factoryMethod);
        }
    }

    Wounds GrimwrathBerzerker::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Unstoppable Berserker
        int threshold = 6;
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) <= 3.0)) threshold--;

        return ignoreWounds(wounds, threshold);
    }

    int GrimwrathBerzerker::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void GrimwrathBerzerker::onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) {
        Fyreslayer::onFriendlyModelSlain(numSlain, attacker, source);

        // Dead, But Not Defeated
        if (numSlain > 0) {
            doPileIn();
            int enemiesSlain = 0;
            fight(numSlain, attacker, enemiesSlain);
        }
    }

    void GrimwrathBerzerker::onEndCombat(PlayerId player) {
        Fyreslayer::onEndCombat(player);

        // Battle-fury
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) < 3.0)) {
            if (Dice::RollD6() >= 2) {
                doPileIn();
                int enemiesSlain = 0;
                fight(1, unit, enemiesSlain);
            }
        }
    }

} // namespace Fyreslayers