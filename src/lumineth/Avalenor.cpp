/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/Avalenor.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace LuminethRealmLords {
    static const int g_basesize = 105; // oval
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 360;

    struct TableEntry {
        int m_blastRange;
        int m_hammerDamage;
        double m_guardianRange;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 10, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {30, 5, 12},
                    {25, 4, 6},
                    {20, 3, 3},
                    {15, 2, 2},
                    {10, 1, 1}
            };

    bool AvalenorTheStoneheartKing::s_registered = false;

    AvalenorTheStoneheartKing::AvalenorTheStoneheartKing(bool isGeneral) :
            LuminethBase(GreatNation::Ymetrica, "Avalenor, the Stoneheart King", 6, g_wounds, 10, 3, false, g_pointsPerUnit),
            m_blast(Weapon::Type::Missile, "Geomantic Blast", 30, 1, 3, 2, -2, RAND_D6),
            m_hammer(Weapon::Type::Melee, "Firestealer Hammers", 2, 6, 3, 3, -1, 5),
            m_hooves(Weapon::Type::Melee, "Cloven Hooves", 1, 2, 3, 3, -1, 2) {
        m_weapons = {&m_blast, &m_hammer, &m_hooves};
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, HERO, ALARITH, YMETRICA, MONSTER, SPIRIT_OF_THE_MOUNTAIN,
                      AVALENOR};
        m_battleFieldRole = Role::Leader_Behemoth;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_blast);
        model->addMeleeWeapon(&m_hammer);
        model->addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    Unit *AvalenorTheStoneheartKing::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new AvalenorTheStoneheartKing(general);
    }

    void AvalenorTheStoneheartKing::Init() {
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

            s_registered = UnitFactory::Register("Avalenor, the Stoneheart King", factoryMethod);
        }
    }

    int AvalenorTheStoneheartKing::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void AvalenorTheStoneheartKing::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_blast.setRange(g_damageTable[damageIndex].m_blastRange);
        m_hammer.setDamage(g_damageTable[damageIndex].m_hammerDamage);

        Unit::onWounded();
    }

    void AvalenorTheStoneheartKing::onRestore() {
        LuminethBase::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    int AvalenorTheStoneheartKing::extraAttacks(const Model *attackingModel, const Weapon *weapon,
                                                const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);

        // All but Immovable
        if (!m_charged && !weapon->isMissile()) extra++;

        return extra;
    }

    size_t AvalenorTheStoneheartKing::getDamageTableIndex() const {

        // Stonemage Symbiosis
        auto mage = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), STONEMAGE, 12.0f);
        if (mage) return 0;

        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds AvalenorTheStoneheartKing::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                                   int woundRoll) const {
        // Firestealer Hammers
        if ((hitRoll == 6) && (weapon->name() == m_hammer.name())) {
            return {weapon->damage(), 1};
        }
        return Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

} // namespace LuminethRealmLords