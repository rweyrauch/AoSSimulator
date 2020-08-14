/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "lumineth/Teclis.h"
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace LuminethRealmLords {

    static const int BASESIZE = 100;
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 660;

    struct TableEntry {
        int m_move;
        int m_talonAttacks;
        int m_auraRange;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {4, 7, 10, 13, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {12, 6, 16},
                    {10,  5,  12},
                    {8,  4,  8},
                    {6,  3,  6},
                    {4,  2,  4}
            };

    bool ArchmageTeclis::s_registered = false;

    Unit *ArchmageTeclis::Create(const ParameterList &parameters) {
        auto unit = new ArchmageTeclis();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int ArchmageTeclis::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void ArchmageTeclis::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Archmage Teclis", factoryMethod);
        }
    }

    ArchmageTeclis::ArchmageTeclis() :
            LuminethBase("Archmage Teclis", 12, WOUNDS, 10, 4, true),
            m_staff(Weapon::Type::Missile, "Lunar Staff", 18, 1, 2, 2, -3, RAND_D3),
            m_sword(Weapon::Type::Melee, "Sword of Teclis", 1, 2, 4, 2, -3, RAND_D3),
            m_talons(Weapon::Type::Melee, "Moonbright Talons", 1, 6, 3, 3, -2, 2) {
        m_weapons = {&m_staff, &m_sword, &m_talons};
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, MONSTER, HERO, WIZARD, TECLIS};
        m_totalUnbinds = INT32_MAX;
        m_totalSpells = 4;
        m_hasMount = true;
        m_battleFieldRole = Role::Leader;

        s_globalCastMod.connect(this, &ArchmageTeclis::auraOfCelennar, &m_auraConnection);
    }

    ArchmageTeclis::~ArchmageTeclis() {
        m_auraConnection.disconnect();
    }

    bool ArchmageTeclis::configure() {

        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_staff);
        model->addMeleeWeapon(&m_sword);
        model->addMeleeWeapon(&m_talons);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);

        return true;
    }

    void ArchmageTeclis::onWounded() {
        Unit::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_move = g_damageTable[damageIndex].m_move;
        m_talons.setAttacks(g_damageTable[damageIndex].m_talonAttacks);
    }

    void ArchmageTeclis::onRestore() {
        Unit::onRestore();
    }

    int ArchmageTeclis::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int ArchmageTeclis::auraOfCelennar(const Unit *caster) {
        // Aura of Celennar
        if (caster->hasKeyword(LUMINETH_REALM_LORDS) &&
            (caster->owningPlayer() == owningPlayer()) && (distanceTo(caster) <= g_damageTable[getDamageTableIndex()].m_auraRange)) {
            return 1;
        }
        return 0;
    }

} // namespace LuminethRealmLords