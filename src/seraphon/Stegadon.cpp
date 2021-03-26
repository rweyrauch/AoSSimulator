/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Stegadon.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 10;
    static const int g_pointsPerUnit = 240;
    static const int POINTS_PER_UNIT_WITH_CHIEF = 250;


    struct TableEntry {
        int m_move;
        int m_hornDamage;
        int m_stompAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 8, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, 4, 5},
                    {7, 3, 4},
                    {6, 2, 3},
                    {5, 2, 2},
                    {4, 1, 1}
            };

    bool Stegadon::s_registered = false;

    Stegadon::Stegadon() :
            SeraphonBase("Stegadon", 8, g_wounds, 5, 4, false),
            m_javelins(Weapon::Type::Missile, "Meteoric Javelins", 8, 4, 5, 4, 0, 1),
            m_bow(Weapon::Type::Missile, "Skystreak Bow", 24, 3, 3, 3, -1, 3),
            m_throwers(Weapon::Type::Missile, "Sunfire Throwers", 8, 1, 0, 0, 0, 0),
            m_warspear(Weapon::Type::Melee, "Meteoric Warspear", 1, 3, 3, 3, -1, 1),
            m_horns(Weapon::Type::Melee, "Massive Horns", 2, 2, 3, 3, -1, 4),
            m_jaws(Weapon::Type::Melee, "Grinding Jaws", 1, 2, 3, 3, -1, 2),
            m_stomps(Weapon::Type::Melee, "Crushing Stomps", 1, 5, 3, 3, -1, 2) {
        m_keywords = {ORDER, SERAPHON, SKINK, MONSTER, STEGADON};
        m_weapons = {&m_javelins, &m_bow, &m_throwers, &m_warspear, &m_horns, &m_jaws, &m_stomps};
        m_battleFieldRole = Role::Behemoth;
        m_hasMount = true;
        m_jaws.setMount(true);
        m_horns.setMount(true);
        m_stomps.setMount(true);
        s_globalBattleshockReroll.connect(this, &Stegadon::steadfastMajestyBraveryReroll, &m_steadfastSlot);
    }

    Stegadon::~Stegadon() {
        m_steadfastSlot.disconnect();
    }

    bool Stegadon::configure(WeaponOption option, bool skinkChief) {
        m_skinkChief = skinkChief;

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_javelins);
        if (option == Skystreak_Bow)
            model->addMissileWeapon(&m_bow);
        else if (option == Sunfire_Throwers)
            model->addMissileWeapon(&m_throwers);
        model->addMeleeWeapon(&m_horns);
        model->addMeleeWeapon(&m_jaws);
        model->addMeleeWeapon(&m_stomps);

        if (m_skinkChief) {
            model->addMeleeWeapon(&m_warspear);
            addKeyword(HERO);
        }
        addModel(model);

        m_points = g_pointsPerUnit;
        if (m_skinkChief) m_points = POINTS_PER_UNIT_WITH_CHIEF;

        return true;
    }

    void Stegadon::onRestore() {
        // Reset table-drive attributes
        onWounded();

        m_armouredCrestAttacker = nullptr;
    }

    Unit *Stegadon::Create(const ParameterList &parameters) {
        auto unit = new Stegadon();
        auto option = (WeaponOption) GetEnumParam("Weapon", parameters, Skystreak_Bow);
        bool chief = GetBoolParam("Skink Chief", parameters, false);

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        unit->setWayOfTheSeraphon(way, constellation);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure(option, chief);
        return unit;
    }

    void Stegadon::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Skystreak_Bow, Sunfire_Throwers};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Weapon", Skystreak_Bow, weapons),
                            BoolParameter("Skink Chief"),
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Stegadon", factoryMethod);
        }
    }

    std::string Stegadon::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Skystreak_Bow) return "Skystreak Bow";
            else if (parameter.intValue == Sunfire_Throwers) return "Sunfire Throwers";
        }
        return SeraphonBase::ValueToString(parameter);
    }

    void Stegadon::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_stomps.setAttacks(g_damageTable[damageIndex].m_stompAttacks);
        m_horns.setDamage(g_damageTable[damageIndex].m_hornDamage);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    size_t Stegadon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int Stegadon::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Skystreak Bow") return Skystreak_Bow;
        else if (enumString == "Sunfire Throwers") return Sunfire_Throwers;

        return SeraphonBase::EnumStringToInt(enumString);
    }

    void Stegadon::onCharged() {
        SeraphonBase::onCharged();

        // Unstoppable Stampede
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto unit : units) {
            if (Dice::RollD6() >= 3) {
                unit->applyDamage({0, Dice::RollD3()}, this);
            }
        }
    }

    Rerolls Stegadon::steadfastMajestyBraveryReroll(const Unit *unit) {
        if (isFriendly(unit) && unit->hasKeyword(SKINK) && (distanceTo(unit) <= 18.0)) return Rerolls::Failed;
        return Rerolls::None;
    }

    int Stegadon::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void Stegadon::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        m_armouredCrestAttacker = nullptr;

        // Armoured Crest
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
            for (auto unit : units) {
                // Select target for Armoured Crest (select first one for now).
                m_armouredCrestAttacker = unit;
                break;
            }
        }
    }

    int Stegadon::targetSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetSaveModifier(weapon, attacker);

        if (attacker == m_armouredCrestAttacker) mod++;

        return mod;
    }

} //namespace Seraphon
