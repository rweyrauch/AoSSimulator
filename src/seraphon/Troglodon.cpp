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

namespace Seraphon {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 260;


    struct TableEntry {
        int m_move;
        int m_spittleRange;
        int m_jawsToWound;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {10, 18, 2},
                    {9,  15, 3},
                    {8,  12, 3},
                    {7,  9,  4},
                    {6,  6,  5}
            };

    bool Troglodon::s_registered = false;

    Troglodon::Troglodon() :
            SeraphonBase("Skink Oracle on Troglodon", 10, WOUNDS, 6, 4, false),
            m_spittle(Weapon::Type::Missile, "Noxious Spittle", 18, RAND_D3, 3, 3, 0, 2),
            m_jaws(Weapon::Type::Melee, "Venomous Jaws", 2, 3, 4, 2, 0, 2),
            m_forelimbs(Weapon::Type::Melee, "Clawed Forelimbs", 2, 2, 4, 3, 0, 2),
            m_rod(Weapon::Type::Melee, "Divining Rod", 1, 2, 4, 4, -1, RAND_D3) {
        m_keywords = {ORDER, SERAPHON, SKINK, MONSTER, HERO, WIZARD, TROGLODON, ORACLE};
        m_weapons = {&m_spittle, &m_jaws, &m_forelimbs, &m_rod};

        s_globalBraveryMod.connect(this, &Troglodon::terror, &m_connection);
    }

    Troglodon::~Troglodon() {
        m_connection.disconnect();
    }

    bool Troglodon::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_spittle);
        model->addMeleeWeapon(&m_jaws);
        model->addMeleeWeapon(&m_forelimbs);
        model->addMeleeWeapon(&m_rod);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    void Troglodon::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    Unit *Troglodon::Create(const ParameterList &parameters) {
        auto unit = new Troglodon();

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, SeraphonBase::Starborne);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, SeraphonBase::None);
        unit->setWayOfTheSeraphon(way, constellation);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
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
                            {ParamType::Enum, "Way of the Seraphon", SeraphonBase::Starborne, SeraphonBase::Starborne,
                             SeraphonBase::Coalesced, 1},
                            {ParamType::Enum, "Constellation", SeraphonBase::None, SeraphonBase::None,
                             SeraphonBase::FangsOfSotek, 1}
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Troglodon", factoryMethod);
        }
    }

    void Troglodon::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_spittle.setRange(g_damageTable[damageIndex].m_spittleRange);
        m_jaws.setToWound(g_damageTable[damageIndex].m_jawsToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int Troglodon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void Troglodon::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (player == owningPlayer()) {
            // Regeneration
            if (Dice::rollD6() >= 2) {
                heal(Dice::rollD3());
            }
        }
    }

    Wounds Troglodon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Venomous Spittle
        if ((woundRoll == 6) && ((weapon->name() == m_spittle.name()) || (weapon->name() == m_jaws.name()))) {
            return {weapon->damage(), 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
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
        return POINTS_PER_UNIT;
    }

} //namespace Seraphon
