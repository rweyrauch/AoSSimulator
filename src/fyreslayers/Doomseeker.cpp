/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/Doomseeker.h>
#include <Board.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool Doomseeker::s_registered = false;

    Doomseeker::Doomseeker() :
            Fyreslayer("Doomseeker", 4, g_wounds, 8, 4, false),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_warIron(Weapon::Type::Melee, "Runic War-iron", 1, 3, 3, 4, 0, 1),
            m_doomseekerAxe(Weapon::Type::Melee, "Doomseeker Axe", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, DOOMSEEKER};
        m_weapons = {&m_throwingAxe, &m_warIron, &m_doomseekerAxe};
        m_battleFieldRole = Leader;
    }

    bool Doomseeker::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMeleeWeapon(&m_warIron);
        model->addMeleeWeapon(&m_doomseekerAxe);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *Doomseeker::Create(const ParameterList &parameters) {
        auto unit = new Doomseeker();

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

    void Doomseeker::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Doomseeker::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    Doomseeker::ComputePoints,
                    {
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                            EnumParameter("Artefact", g_wrathAndDoomArtefacts[0], g_wrathAndDoomArtefacts),
                            BoolParameter("General")
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Doomseeker", factoryMethod);
        }
    }

    Wounds Doomseeker::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Runic Power
        if (!weapon->isMissile()) {
            if (wounds() - remainingWounds() == 1) {
                return {weapon->damage() + 1, 0};
            } else if (wounds() - remainingWounds() >= 2) {
                return {weapon->damage() + 2, 0};
            }
        }
        return Fyreslayer::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Doomseeker::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Fyreslayers