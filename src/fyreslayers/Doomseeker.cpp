/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/Doomseeker.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 90;

    bool Doomseeker::s_registered = false;

    Doomseeker::Doomseeker(Lodge lodge, Artefact artefact) :
            Fyreslayer("Doomseeker", 4, g_wounds, 8, 4, false, g_pointsPerUnit) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, DOOMSEEKER};
        m_weapons = {&m_throwingAxe, &m_warIron, &m_doomseekerAxe};
        m_battleFieldRole = Role::Leader;

        setLodge(lodge);
        setArtefact(artefact);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMeleeWeapon(&m_warIron);
        model->addMeleeWeapon(&m_doomseekerAxe);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *Doomseeker::Create(const ParameterList &parameters) {
        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_wrathAndDoomArtefacts[0]);
        return new Doomseeker(lodge, artefact);
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
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Doomseeker", factoryMethod);
        }
    }

    Wounds Doomseeker::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Runic Power
        if (!weapon->isMissile()) {
            if (wounds() - remainingWounds() == 1) {
                return {weapon->damage() + 1, 0};
            } else if (wounds() - remainingWounds() >= 2) {
                return {weapon->damage() + 2, 0};
            }
        }
        return Fyreslayer::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int Doomseeker::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Fyreslayers