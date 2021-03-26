/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Bastiladon.h>
#include <UnitFactory.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 220;

    struct TableEntry {
        int m_save;
        int m_engineAttacs;
        int m_arkAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 8, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {1, 9, 18},
                    {2, 8, 15},
                    {3, 7, 12},
                    {4, 6, 9},
                    {4, 5, 6}
            };

    bool Bastiladon::s_registered = false;

    Bastiladon::Bastiladon() :
            SeraphonBase("Bastiladon", 5, g_wounds, 6, 1, false),
            m_beam(Weapon::Type::Missile, "Solar Engine", 24, 9, 4, 3, -1, 2),
            m_javelins(Weapon::Type::Missile, "Meteoric Javelins", 8, 4, 5, 4, 0, 1),
            m_ark(Weapon::Type::Melee, "Ark of Sotek", 3, 18, 4, 6, 0, 1),
            m_tail(Weapon::Type::Melee, "Bludgeoning Tail", 2, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, SERAPHON, SKINK, MONSTER, BASTILADON};
        m_weapons = {&m_beam, &m_javelins, &m_ark, &m_tail};
        m_battleFieldRole = Role::Behemoth;
        m_hasMount = true;
        m_tail.setMount(true);
    }

    bool Bastiladon::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_beam);
        model->addMissileWeapon(&m_javelins);
        model->addMeleeWeapon(&m_ark);
        model->addMeleeWeapon(&m_tail);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    Unit *Bastiladon::Create(const ParameterList &parameters) {
        auto unit = new Bastiladon();

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        unit->setWayOfTheSeraphon(way, constellation);

        unit->configure();
        return unit;
    }

    void Bastiladon::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation)
                    },
                    ORDER,
                    {SERAPHON}
            };
            s_registered = UnitFactory::Register("Bastiladon", factoryMethod);
        }
    }

    void Bastiladon::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_beam.setAttacks(g_damageTable[damageIndex].m_engineAttacs);
        m_ark.setAttacks(g_damageTable[damageIndex].m_arkAttacks);
        m_save = g_damageTable[getDamageTableIndex()].m_save;
    }

    size_t Bastiladon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds Bastiladon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Light of the Heavens
        if ((target->hasKeyword(DAEMON) && target->hasKeyword(CHAOS) && (weapon->name() == m_beam.name()))) {
            return {weapon->damage() + 1, 0};
        }

        // Tide of Snakes
        if ((hitRoll == 6) && (weapon->name() == m_ark.name())) {
            return {0, 1};
        }
        return SeraphonBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void Bastiladon::onRestore() {
        SeraphonBase::onRestore();

        // Reset table attributes
        onWounded();
    }

    int Bastiladon::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Seraphon
