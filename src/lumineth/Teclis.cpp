/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "lumineth/Teclis.h"
#include "LuminethPrivate.h"
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace LuminethRealmLords {

    static const int g_basesize = 100;
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 660;

    struct TableEntry {
        int m_move;
        int m_talonAttacks;
        int m_auraRange;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {4, 7, 10, 13, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
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

        auto nation = (GreatNation)GetEnumParam("Nation", parameters, (int)GreatNation::None);
        unit->setNation(nation);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int ArchmageTeclis::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void ArchmageTeclis::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General"),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Archmage Teclis", factoryMethod);
        }
    }

    ArchmageTeclis::ArchmageTeclis() :
            LuminethBase("Archmage Teclis", 12, g_wounds, 10, 4, true),
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

        auto model = new Model(g_basesize, wounds());
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
        LuminethBase::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    int ArchmageTeclis::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
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