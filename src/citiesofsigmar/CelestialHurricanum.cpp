/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "citiesofsigmar/CelestialHurricanum.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 105;
    static const int g_wounds = 11;
    static const int g_pointsPerUnit = 220;
    static const int g_pointsPerUnitWithBattlemage = 280;

    struct TableEntry {
        int m_move;
        double m_portentsOfBattle;
        int m_stormOfShemtek;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 8, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 10, 3},
                    {9,  8,  2},
                    {8,  6,  2},
                    {7,  4,  1},
                    {6,  2,  1}
            };

    bool CelestialHurricanum::s_registered = false;

    Unit *CelestialHurricanum::Create(const ParameterList &parameters) {
        auto unit = new CelestialHurricanum();

        auto battlemage = GetBoolParam("Battlemage", parameters, true);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(battlemage, lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string CelestialHurricanum::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int CelestialHurricanum::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void CelestialHurricanum::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("City", g_city[0], g_city),
                            BoolParameter("Battlemage"),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };

            s_registered = UnitFactory::Register("Celestial Hurricanum", factoryMethod);
        }
    }

    CelestialHurricanum::CelestialHurricanum() :
            CitizenOfSigmar("Celestial Hurricanum", 10, g_wounds, 6, 4, false),
            m_stormOfShemtek(Weapon::Type::Missile, "Storm of Shemtek", 18, 1, 0, 0, 0, 0),
            m_wizardStaff(Weapon::Type::Melee, "Wizard's Staff", 2, 1, 4, 3, -1, RAND_D3),
            m_arcaneTools(Weapon::Type::Melee, "Arcane Tools", 1, 4, 5, 5, 0, 1),
            m_hooves(Weapon::Type::Melee, "Steel-shod Hooves", 1, 4, 4, 4, 0, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, COLLEGIATE_ARCANE, CELESTIAL_HURRICANUM};
        m_weapons = {&m_stormOfShemtek, &m_wizardStaff, &m_arcaneTools, &m_hooves};
        m_hasMount = true;
        m_battleFieldRole = Behemoth;

        s_globalCastMod.connect(this, &CelestialHurricanum::locusOfAzyr, &m_locusConnection);
        s_globalToHitMod.connect(this, &CelestialHurricanum::portentsOfBattle, &m_portentsConnection);
    }

    CelestialHurricanum::~CelestialHurricanum() {
        m_locusConnection.disconnect();
        m_portentsConnection.disconnect();
    }

    bool CelestialHurricanum::configure(bool battlemage, Lore lore) {
        if (battlemage) {
            addKeyword(WIZARD);
            addKeyword(HERO);
            m_battleFieldRole = Leader_Behemoth;
            m_totalSpells = 1;
            m_totalUnbinds = 1;
        }

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_stormOfShemtek);
        model->addMeleeWeapon(&m_arcaneTools);
        model->addMeleeWeapon(&m_hooves);
        if (battlemage) {
            model->addMeleeWeapon(&m_wizardStaff);
        }
        addModel(model);

        if (battlemage) {
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
            m_knownSpells.push_back(std::make_unique<MysticShield>(this));
            m_points = g_pointsPerUnitWithBattlemage;
        } else {
            m_points = g_pointsPerUnit;
        }

        return true;
    }

    void CelestialHurricanum::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    void CelestialHurricanum::onWounded() {
        Unit::onWounded();

        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int CelestialHurricanum::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void CelestialHurricanum::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        if (m_shootingTarget) {
            // Storm of Shemtek
            auto dist = distanceTo(m_shootingTarget);
            if (dist <= (double) m_stormOfShemtek.range()) {
                Dice::RollResult result;
                Dice::RollD6(g_damageTable[getDamageTableIndex()].m_stormOfShemtek, result);
                auto numHits = result.rollsGE(2);
                for (auto i = 0; i < numHits; i++) {
                    auto mw = Dice::RollD3();
                    m_shootingTarget->applyDamage({0, mw});
                }
            }
        }
    }

    int CelestialHurricanum::castingModifier() const {
        auto mod = Unit::castingModifier();

        // Celestial Battlemage
        if (Board::Instance()->getRealm() == Azyr) mod++;

        return mod;
    }

    int CelestialHurricanum::locusOfAzyr(const Unit *caster) {
        // Locus of Azyr
        if (caster->hasKeyword(COLLEGIATE_ARCANE) && caster->hasKeyword(WIZARD) &&
            (caster->owningPlayer() == owningPlayer()) && (distanceTo(caster) <= 12.0)) {
            return 1;
        }
        return 0;
    }

    int CelestialHurricanum::portentsOfBattle(const Unit * /*attacker*/, const Weapon * /*weapon*/, const Unit *unit) {
        // Portents of Battle
        if (unit->hasKeyword(CITIES_OF_SIGMAR) &&
            (unit->owningPlayer() == owningPlayer()) &&
            (distanceTo(unit) <= g_damageTable[getDamageTableIndex()].m_portentsOfBattle)) {
            return 1;
        }
        return 0;
    }

    int CelestialHurricanum::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace CitiesOfSigmar