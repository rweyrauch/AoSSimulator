/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Board.h>
#include "freeagents/Kragnos.h"

namespace Destruction {
    static const int g_basesize = 130;
    static const int g_wounds = 18;
    static const int g_pointsPerUnit = 800;

    struct TableEntry {
        int m_move;
        int m_hoovesAttacks;
        int m_bellowOfRage;
    };

    const size_t g_numTableEntries = 4;
    static int g_woundThresholds[g_numTableEntries] = {9, 12, 15, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 6,  5},
                    {9, 5,  4},
                    {8,  4, 3},
                    {7,  3, 2},
            };

    bool Kragnos::s_registered = false;

    Unit *Kragnos::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new Kragnos(general);
    }

    int Kragnos::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void Kragnos::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Kragnos::Create,
                    nullptr,
                    nullptr,
                    Kragnos::ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ, SONS_OF_BEHEMAT, IRONJAWZ, OGOR_MAWTRIBES, BONESPLITTERZ}
            };
            s_registered = UnitFactory::Register("Kragnos", factoryMethod);
        }
    }

    Kragnos::Kragnos(bool isGeneral) :
        Unit("Kragnos", 10, g_wounds, 10, 2, false, g_pointsPerUnit) {

        m_keywords = {DESTRUCTION, MONSTER, HERO, KRAGNOS};
        m_weapons = {&m_mace, &m_tuskbreaker, &m_hooves};
        m_battleFieldRole = Role::Leader_Behemoth;

        s_globalBraveryMod.connect(this, &Kragnos::iconOfDestruction, &m_iconOfDestruction);

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_mace);
        model->addMeleeWeapon(&m_tuskbreaker);
        model->addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    Kragnos::~Kragnos() {
        m_iconOfDestruction.disconnect();
    }

    size_t Kragnos::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void Kragnos::onWounded() {
        Unit::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_hooves.setAttacks(g_damageTable[damageIndex].m_hoovesAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void Kragnos::onRestore() {
        Unit::onRestore();

        // Reset table-drive attributes
        onWounded();
    }

    Rerolls Kragnos::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Destroyer of Draconith Empire
        auto stardrakes = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), STARDRAKE, 12.0);
        auto drake = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), DRAKE, 12.0);
        auto dracoth = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), DRACOTH, 12.0);
        auto dracoline = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), DRACOLINE, 12.0);
        if ((stardrakes && (stardrakes->remainingModels() > 0)) || (drake && drake->remainingModels() > 0) ||
            (dracoth && dracoth->remainingModels() > 0) || (dracoline && dracoline->remainingModels() > 0)) {
            return Rerolls::Failed;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    Rerolls Kragnos::chargeRerolls() const {
        // Destroyer of Draconith Empire
        auto stardrakes = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), STARDRAKE, 12.0);
        auto drake = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), DRAKE, 12.0);
        auto dracoth = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), DRACOTH, 12.0);
        auto dracoline = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), DRACOLINE, 12.0);
        if ((stardrakes && (stardrakes->remainingModels() > 0)) || (drake && drake->remainingModels() > 0) ||
            (dracoth && dracoth->remainingModels() > 0) || (dracoline && dracoline->remainingModels() > 0)) {
            return Rerolls::Failed;
        }
        return Unit::chargeRerolls();
    }

    void Kragnos::onCharged() {
        // Rampaging Destruction
        auto monster = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), MONSTER, 1.0);
        if (monster && (monster->remainingModels() > 0)) {
            auto roll1 = Dice::RollD6();
            auto roll2 = Dice::RollD6();
            if (roll1 + roll2 != 7) {
                monster->applyDamage({0, roll1*roll2, Wounds::Source::Ability}, this);
            }
        }
        else {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
            for (auto unit : units) {
                if (Dice::RollD6() >= 2) {
                    unit->applyDamage({0, Dice::RollD6(), Wounds::Source::Ability}, this);
                }
            }
        }
        Unit::onCharged();
    }

    int Kragnos::iconOfDestruction(const Unit *target) {
        // Icon of Destruction
        if (target->hasKeyword(DESTRUCTION) && (target->owningPlayer() == owningPlayer()) && (distanceTo(target) <= 12.0)) {
            return 1;
        }
        return 0;
    }

    Wounds Kragnos::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // The Shield Inviolate
        if ((wounds.source == Wounds::Source::Spell) && (wounds.sourceObject != nullptr)) {
            auto spell = reinterpret_cast<const Spell*>(wounds.sourceObject);
            auto roll = Dice::Roll3D6();
            if (roll > spell->castingValue()) {
                return {0, 0, Wounds::Source::Unknown, nullptr};
            }
        }
        return Unit::applyWoundSave(wounds, attackingUnit);
    }

    void Kragnos::onEndHero(PlayerId player) {
        Unit::onEndHero(player);
        bellowOfRage();
    }

    void Kragnos::onEndMovement(PlayerId player) {
        Unit::onEndMovement(player);
        bellowOfRage();
    }

    void Kragnos::onEndShooting(PlayerId player) {
        Unit::onEndShooting(player);
        bellowOfRage();
    }

    void Kragnos::onEndCombat(PlayerId player) {
        EventInterface::onEndCombat(player);
        bellowOfRage();
    }

    void Kragnos::bellowOfRage() {
        // Bellow of Rage
        if (remainingWounds() < m_woundsAtStartOfPhase) {
            auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 6.0);
            for (auto unit : units) {
                const auto damageIndex = getDamageTableIndex();
                if (Dice::RollD6() >= g_damageTable[damageIndex].m_bellowOfRage) {
                    unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Ability, nullptr}, this);
                }
            }
        }
    }

} // namespace Destruction
