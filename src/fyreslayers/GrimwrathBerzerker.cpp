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
    static const int g_pointsPerUnit = 110;

    bool GrimwrathBerzerker::s_registered = false;

    GrimwrathBerzerker::GrimwrathBerzerker(Lodge lodge, Artefact artefact) :
            Fyreslayer("Grimwrath Berzerker", 4, g_wounds, 9, 4, false, g_pointsPerUnit) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, GRIMWRATH_BERZERKER};
        m_weapons = {&m_throwingAxe, &m_greatAxe};

        setLodge(lodge);
        setArtefact(artefact);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMeleeWeapon(&m_greatAxe);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *GrimwrathBerzerker::Create(const ParameterList &parameters) {
        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_wrathAndDoomArtefacts[0]);
        return new GrimwrathBerzerker(lodge, artefact);
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
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Grimwrath Berzerker", factoryMethod);
        }
    }

    int GrimwrathBerzerker::ComputePoints(const ParameterList& /*parameters*/) {
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