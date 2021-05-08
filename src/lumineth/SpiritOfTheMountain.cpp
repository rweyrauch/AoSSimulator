/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/SpiritOfTheMountain.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {
    static const int g_basesize = 105; // oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 340;

    struct TableEntry {
        int m_blastRange;
        int m_hammerDamage;
        double m_shockwave;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 10, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {30, 5, 12},
                    {25, 4, 10},
                    {20, 3, 8},
                    {15, 2, 6},
                    {10, 1, 4}
            };

    bool AlarithSpiritOfTheMountain::s_registered = false;

    AlarithSpiritOfTheMountain::AlarithSpiritOfTheMountain(GreatNation nation, CommandTrait trait, Artefact artefact, bool isGeneral) :
            LuminethBase(nation, "Alarith Spirit of the Mountain", 6, g_wounds, 10, 3, false, g_pointsPerUnit),
            m_blast(Weapon::Type::Missile, "Geomantic Blast", 30, 1, 3, 2, -2, RAND_D6),
            m_hammer(Weapon::Type::Melee, "Stoneheart Worldhammer", 3, 4, 3, 2, -2, 5),
            m_hooves(Weapon::Type::Melee, "Cloven Hooves", 1, 2, 3, 3, -1, 2) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, HERO, ALARITH, MONSTER, SPIRIT_OF_THE_MOUNTAIN};
        m_weapons = {&m_blast, &m_hammer, &m_hooves};
        m_battleFieldRole = Role::Leader_Behemoth;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_blast);
        model->addMeleeWeapon(&m_hammer);
        model->addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    Unit *AlarithSpiritOfTheMountain::Create(const ParameterList &parameters) {
        auto nation = (GreatNation) GetEnumParam("Nation", parameters, (int) GreatNation::None);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraitsAlarith[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_commandTraitsAlarith[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new AlarithSpiritOfTheMountain(nation, trait, artefact, general);
    }

    void AlarithSpiritOfTheMountain::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General"),
                            EnumParameter("Command Trait", g_commandTraitsAlarith[0], g_commandTraitsAlarith),
                            EnumParameter("Artefact", g_artefactsAlarith[0], g_artefactsAlarith),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Alarith Spirit of the Mountain", factoryMethod);
        }
    }

    int AlarithSpiritOfTheMountain::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void AlarithSpiritOfTheMountain::onRestore() {
        LuminethBase::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    void AlarithSpiritOfTheMountain::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_blast.setRange(g_damageTable[damageIndex].m_blastRange);
        m_hammer.setDamage(g_damageTable[damageIndex].m_hammerDamage);

        LuminethBase::onWounded();
    }

    size_t AlarithSpiritOfTheMountain::getDamageTableIndex() const {

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

    int AlarithSpiritOfTheMountain::extraAttacks(const Model *attackingModel, const Weapon *weapon,
                                                 const Unit *target) const {
        auto extra = LuminethBase::extraAttacks(attackingModel, weapon, target);

        // All but Immovable
        if (!m_charged && !weapon->isMissile()) extra++;

        return extra;
    }

    void AlarithSpiritOfTheMountain::onStartShooting(PlayerId player) {
        LuminethBase::onStartShooting(player);

        // Stoneheart Shockwave
        if (player != owningPlayer()) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()),
                                                           g_damageTable[getDamageTableIndex()].m_shockwave);
            for (auto ip : units) {
                double maxRange = 0.0;
                if (ip->hasShootingAttack(maxRange)) {
                    ip->buffModifier(Attribute::To_Hit_Missile, -1, {GamePhase::Shooting, m_battleRound, player});
                    break;
                }
            }
        }
    }

    void AlarithSpiritOfTheMountain::onStartCombat(PlayerId player) {
        LuminethBase::onStartCombat(player);

        // Stoneheart Shockwave
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()),
                                                       g_damageTable[getDamageTableIndex()].m_shockwave);
        for (auto ip : units) {
            ip->buffModifier(Attribute::To_Hit_Melee, -1, {GamePhase::Combat, m_battleRound, player});
            break;
        }
    }

} // namespace LuminethRealmLords