/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Troglodon.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "SeraphonPrivate.h"
#include "SeraphonLore.h"

namespace Seraphon {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 220;


    struct TableEntry {
        int m_move;
        int m_spittleRange;
        int m_jawsToWound;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 18, 2},
                    {9,  15, 3},
                    {8,  12, 3},
                    {7,  9,  4},
                    {6,  6,  5}
            };

    bool Troglodon::s_registered = false;

    Troglodon::Troglodon() :
            SeraphonBase("Skink Oracle on Troglodon", 10, g_wounds, 6, 4, false),
            m_spittle(Weapon::Type::Missile, "Noxious Spittle", 18, RAND_D3, 3, 3, 0, 2),
            m_jaws(Weapon::Type::Melee, "Venomous Jaws", 2, 3, 4, 2, 0, 2),
            m_forelimbs(Weapon::Type::Melee, "Clawed Forelimbs", 2, 2, 4, 3, 0, 2),
            m_rod(Weapon::Type::Melee, "Divining Rod", 1, 2, 4, 4, -1, RAND_D3) {
        m_keywords = {ORDER, SERAPHON, SKINK, MONSTER, HERO, WIZARD, TROGLODON, ORACLE};
        m_weapons = {&m_spittle, &m_jaws, &m_forelimbs, &m_rod};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_spittle.setMount(true);
        m_jaws.setMount(true);
        m_forelimbs.setMount(true);
        m_totalSpells = 1;
        m_totalUnbinds = 1;

        s_globalBraveryMod.connect(this, &Troglodon::terror, &m_connection);
    }

    Troglodon::~Troglodon() {
        m_connection.disconnect();
    }

    void Troglodon::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_spittle);
        model->addMeleeWeapon(&m_jaws);
        model->addMeleeWeapon(&m_forelimbs);
        model->addMeleeWeapon(&m_rod);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateCometsCall(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);
    }

    void Troglodon::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    Unit *Troglodon::Create(const ParameterList &parameters) {
        auto unit = new Troglodon();

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        unit->setWayOfTheSeraphon(way, constellation);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfCelestialManipulation[0]);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_skinkCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_vestmentsOfThePriesthood[0]);

        unit->setArtefact(artefact);
        unit->setCommandTrait(trait);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure(lore);
        return unit;
    }

    void Troglodon::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation),
                            EnumParameter("Command Trait", g_skinkCommandTrait[0], g_skinkCommandTrait),
                            EnumParameter("Artefact", g_vestmentsOfThePriesthood[0], g_vestmentsOfThePriesthood),
                            EnumParameter("Lore", g_loreOfCelestialManipulation[0], g_loreOfCelestialManipulation),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Troglodon", factoryMethod);
        }
    }

    void Troglodon::onWounded() {
        SeraphonBase::onWounded();
        const int damageIndex = getDamageTableIndex();
        m_spittle.setRange(g_damageTable[damageIndex].m_spittleRange);
        m_jaws.setToWound(g_damageTable[damageIndex].m_jawsToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    size_t Troglodon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void Troglodon::onStartHero(PlayerId player) {
        SeraphonBase::onStartHero(player);

        if (player == owningPlayer()) {
            // Regeneration
            if (Dice::RollD6() >= 2) {
                heal(Dice::RollD3());
            }
        }
    }

    Wounds Troglodon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Venomous Spittle
        if ((woundRoll == 6) && ((weapon->name() == m_spittle.name()) || (weapon->name() == m_jaws.name()))) {
            return {weapon->damage(), 1};
        }
        return SeraphonBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Troglodon::castingModifier() const {
        auto mod = SeraphonBase::castingModifier();
        // Oracle of the Slann
        mod++;
        return mod;
    }

    int Troglodon::terror(const Unit *target) {
        // Terror
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }
        return 0;
    }

    int Troglodon::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Seraphon
