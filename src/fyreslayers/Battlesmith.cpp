/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/Battlesmith.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool Battlesmith::s_registered = false;

    Battlesmith::Battlesmith(Lodge lodge, Artefact artefact, bool isGeneral) :
            Fyreslayer(lodge, "Battlesmith", 4, g_wounds, 7, 4, false, g_pointsPerUnit),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_battleAxe(Weapon::Type::Melee, "Ancestral Battle-axe", 1, 3, 3, 3, -1, 2) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, TOTEM, BATTLESMITH};
        m_weapons = {&m_throwingAxe, &m_battleAxe};
        m_battleFieldRole = Role::Leader;

        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMeleeWeapon(&m_battleAxe);
        addModel(model);
    }

    Unit *Battlesmith::Create(const ParameterList &parameters) {
        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_iconArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Battlesmith(lodge, artefact, general);
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

    int Battlesmith::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Fyreslayers