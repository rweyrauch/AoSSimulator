/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/StarbloodStalkers.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_pointsPerUnit = 0;

    bool StarbloodStalkers::s_registered = false;

    StarbloodStalkers::StarbloodStalkers() :
            SeraphonBase("The Starblood Stalkers", 8, g_wounds, 5, 6, false) {
        m_keywords = {ORDER, SERAPHON, COALESCED, THUNDER_LIZARD, SKINKS, STARBLOOD_STALKERS};
        m_weapons = {&m_javelin, &m_dartpipe, &m_boltspitter, &m_dagger, &m_club};

        setWayOfTheSeraphon(WayOfTheSeraphon::Coalesced, Constellation::Thunder_Lizard);

        auto xepic = new Model(g_basesize, wounds());
        xepic->addMeleeWeapon(&m_club);
        xepic->setName("Xepic");
        addModel(xepic);

        auto huachi = new Model(g_basesize, wounds());
        huachi->addMissileWeapon(&m_javelin);
        huachi->addMeleeWeapon(&m_dagger);
        huachi->setName("Huachi");
        addModel(huachi);

        auto tok = new Model(g_basesize, wounds());
        tok->addMissileWeapon(&m_boltspitter);
        tok->addMeleeWeapon(&m_club);
        tok->setName("Tok");
        addModel(tok);

        auto otapatl = new Model(g_basesize, wounds());
        otapatl->addMissileWeapon(&m_dartpipe);
        otapatl->addMeleeWeapon(&m_dagger);
        otapatl->setName("Otapatl");
        addModel(otapatl);

        m_points = ComputePoints(4);
    }

    Unit *StarbloodStalkers::Create(const ParameterList &parameters) {
        return new StarbloodStalkers();
    }

    void StarbloodStalkers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("The Starblood Stalkers", factoryMethod);
        }
    }

    int StarbloodStalkers::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = SeraphonBase::toSaveModifier(weapon, attacker);

        // Star-bucklers
        modifier++;

        return modifier;
    }

    int StarbloodStalkers::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    int StarbloodStalkers::braveryModifier() const {
        auto mod = SeraphonBase::braveryModifier();

        // Bolstering Presence (from Klaq-Trok warscroll)
        auto klaq = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), KLAQ_TROK, 9.0);
        if (klaq && (klaq->remainingModels() > 0)) {
            mod++;
        }
        return mod;
    }

    Wounds StarbloodStalkers::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Star-venom
        if ((hitRoll == 6) && (weapon->name() == m_dartpipe.name())) {
            return {0, 1, Wounds::Source::Weapon_Missile};
        }
        return SeraphonBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

} //namespace Seraphon
