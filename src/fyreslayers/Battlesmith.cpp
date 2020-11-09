/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/Battlesmith.h>
#include <Board.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool Battlesmith::s_registered = false;

    Battlesmith::Battlesmith() :
            Fyreslayer("Battlesmith", 4, g_wounds, 7, 4, false),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_battleAxe(Weapon::Type::Melee, "Ancestral Battle-axe", 1, 3, 3, 3, -1, 2) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, TOTEM, BATTLESMITH};
        m_weapons = {&m_throwingAxe, &m_battleAxe};
        m_battleFieldRole = Leader;
    }

    bool Battlesmith::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMeleeWeapon(&m_battleAxe);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *Battlesmith::Create(const ParameterList &parameters) {
        auto unit = new Battlesmith();

        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        unit->setLodge(lodge);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_iconArtefacts[0]);
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

    void Battlesmith::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Battlesmith::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    Battlesmith::ComputePoints,
                    {
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                            EnumParameter("Artefact", g_iconArtefacts[0], g_iconArtefacts),
                            BoolParameter("General")
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Battlesmith", factoryMethod);
        }
    }

    int Battlesmith::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Fyreslayers