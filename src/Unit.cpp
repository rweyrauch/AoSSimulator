/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <cfloat>
#include <algorithm>
#include <utility>
#include <Unit.h>
#include <Dice.h>
#include <Board.h>
#include <Roster.h>
#include <Think.h>
#include <plog/Log.h>

const double MIN_CHARGE_DISTANCE = 3.0;

lsignal::signal<int(const Unit *)> Unit::s_globalMoveMod;
lsignal::signal<int(const Unit *)> Unit::s_globalRunMod;
lsignal::signal<int(const Unit *)> Unit::s_globalChargeMod;

lsignal::signal<int(const Unit *)> Unit::s_globalBraveryMod;
lsignal::signal<int(const Unit *, const Weapon *, const Unit *)> Unit::s_globalToHitMod;
lsignal::signal<int(const Unit *, const Weapon *, const Unit *)> Unit::s_globalToWoundMod;
lsignal::signal<int(const Unit *, const Weapon *)> Unit::s_globalSaveMod;

lsignal::signal<int(const Unit *, const Model *, const Weapon *, const Unit *)> Unit::s_globalAttackMod;

lsignal::signal<int(const Unit *)> Unit::s_globalCastMod;
lsignal::signal<Rerolls(const Unit* caster)> Unit::s_globalCastReroll;
lsignal::signal<int(const Unit *)> Unit::s_globalUnbindMod;

lsignal::signal<Rerolls(const Unit *, const Weapon *, const Unit *)> Unit::s_globalToHitReroll;
lsignal::signal<Rerolls(const Unit *, const Weapon *, const Unit *)> Unit::s_globalToWoundReroll;
lsignal::signal<Rerolls(const Unit *, const Weapon *, const Unit *)> Unit::s_globalSaveReroll;

lsignal::signal<Rerolls(const Unit *)> Unit::s_globalBattleshockReroll;

lsignal::signal<int(const Unit *, int roll)> Unit::s_globalBattleshockFleeModifier;
lsignal::signal<Rerolls(const Unit *)> Unit::s_globalRunReroll;
lsignal::signal<Rerolls(const Unit *)> Unit::s_globalChargeReroll;

lsignal::signal<Wounds(const Wounds &wounds, const Unit *target, const Unit *attacker)> Unit::s_globalWoundSave;

static int accumulate(const std::vector<int> &v) {
    return std::accumulate(v.cbegin(), v.cend(), 0);
}


Unit::Unit() {
    // Populate all buff/ability maps with empty lists
    constexpr auto &attributeEntries = magic_enum::enum_entries<Attribute>();
    constexpr auto &movementEntries = magic_enum::enum_entries<MovementRule>();
    constexpr auto &abilityEntries = magic_enum::enum_entries<Ability>();

    for (auto entry : attributeEntries) {
        m_attributeModifiers[entry.first] = std::list<ModifierBuff>();
        m_rollModifiers[entry.first] = std::list<RerollBuff>();
    }
    for (auto entry : movementEntries) {
        m_movementRules[entry.first] = std::list<MovementRuleBuff>();
    }
    for (auto entry : abilityEntries) {
        m_abilityBuffs[entry.first] = std::list<AbilityBuff>();
    }
}

Wounds Unit::shoot(int numAttackingModels, Unit *targetUnit, int &numSlain) {
    if ((targetUnit == nullptr) || (remainingModels() == 0)) {
        return {0, 0, Wounds::Source::Weapon_Missile};
    }
    if (m_ran && !canRunAndShoot()) {
        return {0, 0, Wounds::Source::Weapon_Missile};
    }

    if ((numAttackingModels == -1) || (numAttackingModels > (int) m_models.size())) {
        numAttackingModels = (int) m_models.size();
    }

    PLOG_INFO << name() << " shooting at " << targetUnit->name() << " with " << numAttackingModels << " model(s).";

    Wounds totalDamage = {0, 0, Wounds::Source::Weapon_Missile};
    Wounds totalDamageReflected = {0, 0, Wounds::Source::Weapon_Missile};

    for (auto i = 0; i < numAttackingModels; i++) {
        auto model = m_models.at(i).get();
        if (model->fled() || model->slain()) continue;

        for (auto wip = model->missileWeaponBegin(); wip != model->missileWeaponEnd(); ++wip) {
            const Weapon *w = *wip;

            Wounds weaponDamage, reflectedDamage;
            attackWithWeapon(w, targetUnit, model, weaponDamage, reflectedDamage);

            if (!weaponDamage.zero()) {
                PLOG_INFO << "\tModel[" << i << "] shoots with " << w->name() << " doing damage " << weaponDamage;
            }

            totalDamage += weaponDamage;
            totalDamageReflected += reflectedDamage;
        }
    }

    // Apply all of the wounds to the target.  Target apply wound save and return
    // number of slain models.
    numSlain = targetUnit->applyDamage(totalDamage, this);
    if (numSlain > 0) {
        PLOG_INFO << name() << " slays " << numSlain << " enemy model(s) from " << targetUnit->name()
                  << " in shooting phase.";
        onEnemyModelSlain(numSlain, targetUnit, totalDamage.source);

        if (targetUnit->hasKeyword(MONSTER)) {
            m_currentRecord.m_monstersSlain++;
        }
        if (targetUnit->hasKeyword(HERO)) {
            m_currentRecord.m_herosSlain++;
        }
    }

    if (targetUnit->remainingModels() == 0) {
        PLOG_INFO << name() << " slays enemy unit " << targetUnit->name() << " in shooting phase.";
        targetUnit->onFriendlyUnitSlain(nullptr);
        onEnemyUnitSlain(targetUnit);
    }

    // apply returned damage to this unit
    int numSlainByReturnedDamage = applyDamage(totalDamageReflected, targetUnit);
    if (numSlainByReturnedDamage > 0) {
        PLOG_INFO << "Enemy unit " << targetUnit->name() << " slays " << numSlain << " friently model(s) from "
                  << name() << " with returned shooting damage.";
        targetUnit->onEnemyModelSlain(numSlain, this, totalDamageReflected.source);
    }

    if (remainingModels() == 0) {
        PLOG_INFO << "Enemy unit " << targetUnit->name() << " slays friendly unit " << name()
                  << " with returned shooting damage.";
        onFriendlyUnitSlain(nullptr);
        targetUnit->onEnemyUnitSlain(this);
    }

    m_currentRecord.m_woundsInflicted += totalDamage;
    m_currentRecord.m_woundsTaken += totalDamageReflected;
    m_currentRecord.m_enemyModelsSlain += numSlain;
    m_currentRecord.m_modelsSlain += numSlainByReturnedDamage;

    return totalDamage;
}


Wounds Unit::fight(int numAttackingModels, Unit *targetUnit, int &numSlain) {
    if (targetUnit == nullptr) {
        numSlain = 0;
        return {0, 0, Wounds::Source::Weapon_Melee};
    }
    if (remainingModels() == 0) {
        numSlain = 0;
        return {0, 0, Wounds::Source::Weapon_Melee};
    }
    if ((numAttackingModels == -1) || (numAttackingModels > (int) m_models.size())) {
        numAttackingModels = (int) m_models.size();
    }

    doPileIn();

    Wounds totalDamage = {0, 0, Wounds::Source::Weapon_Melee};
    Wounds totalDamageReflected = {0, 0, Wounds::Source::Weapon_Melee};

    PLOG_INFO << name() << " attacking " << targetUnit->name() << " with " << numAttackingModels << " model(s).";

    for (auto i = 0; i < numAttackingModels; i++) {
        const auto model = m_models.at(i).get();
        if (model->fled() || model->slain()) continue;

        for (auto wip = model->meleeWeaponBegin(); wip != model->meleeWeaponEnd(); ++wip) {
            const Weapon *w = *wip;

            Wounds weaponDamage, reflectedDamage;
            attackWithWeapon(w, targetUnit, model, weaponDamage, reflectedDamage);

            if (!weaponDamage.zero()) {
                PLOG_INFO << "\tModel[" << i << "] attacks with " << w->name() << " doing damage " << weaponDamage;

                auto slainWithWeapon = targetUnit->applyDamage(weaponDamage, this);
                if (slainWithWeapon) {
                    onEnemyModelSlainWithWeapon(slainWithWeapon, targetUnit, w, weaponDamage);
                    PLOG_INFO << name() << " slays " << slainWithWeapon << " enemy model(s) from " << targetUnit->name()
                              << " in combat phase with weapon " << w->name();
                }
                else {
                    onEnemyModelWoundedWithWeapon(targetUnit, w, weaponDamage);
                }
                numSlain += slainWithWeapon;
            }

            totalDamage += weaponDamage;
            totalDamageReflected += reflectedDamage;
        }
    }

    // Some units do mortal wounds for just existing!  See Evocators for example.
    auto generatedMortals = generateMortalWounds(targetUnit);
    if (generatedMortals > 0) {
        auto slainWithMortal = targetUnit->applyDamage({0, generatedMortals, Wounds::Source::Ability}, this);
        if (slainWithMortal) {
            // TODO: invoke event method here?
        }
        numSlain += slainWithMortal;
        totalDamage.mortal += generatedMortals;
    }

    if (numSlain > 0) {
        PLOG_INFO << name() << " slays " << numSlain << " enemy model(s) from " << targetUnit->name()
                  << " in combat phase.";
        onEnemyModelSlain(numSlain, targetUnit, totalDamage.source);

        if (targetUnit->hasKeyword(MONSTER)) {
            m_currentRecord.m_monstersSlain++;
        }
        if (targetUnit->hasKeyword(HERO)) {
            m_currentRecord.m_herosSlain++;
        }
    }

    if (targetUnit->remainingModels() == 0) {
        PLOG_INFO << name() << " slays enemy unit " << targetUnit->name() << " in combat phase.";
        targetUnit->onFriendlyUnitSlain(nullptr);
        onEnemyUnitSlain(targetUnit);
    }

    int numSlainByReturnedDamage = applyDamage(totalDamageReflected, targetUnit);
    if (numSlainByReturnedDamage > 0) {
        PLOG_INFO << "Enemy unit " << targetUnit->name() << " slays " << numSlain << " friently model(s) from "
                  << name() << " with returned damage.";
        targetUnit->onEnemyModelSlain(numSlainByReturnedDamage, this, totalDamageReflected.source);
    }

    if (remainingModels() == 0) {
        PLOG_INFO << "Enemy unit " << targetUnit->name() << " slays friendly unit " << name()
                  << " with returned damage.";
        onFriendlyUnitSlain(nullptr);
        targetUnit->onEnemyUnitSlain(this);
    }

    m_currentRecord.m_woundsInflicted += totalDamage;
    m_currentRecord.m_woundsTaken += totalDamageReflected;
    m_currentRecord.m_enemyModelsSlain += numSlain;
    m_currentRecord.m_modelsSlain += numSlainByReturnedDamage;

    m_hasFought = true;

    return totalDamage;
}

int Unit::applyBattleshock() {

    if (!battleshockRequired()) return 0;
    if (m_modelsSlain <= 0) return 0;
    if (remainingModels() == 0) return 0;

    // TODO: implement rerolls for battleshock.
    auto roll = rollBattleshock();
    int numFled, numRestored;
    computeBattleshockEffect(roll, numFled, numRestored);

    // mark fleeing models
    int numFleeing = numFled;

    for (auto ip = m_models.rbegin(); ip != m_models.rend() && numFleeing > 0; ++ip) {
        Model *model = ip->get();
        if (model->fled() || model->slain()) continue;
        model->flee();
        onModelFled(model);
        numFleeing--;
    }

    if (numFled > 0) {
        PLOG_INFO << name() << " failed battleshock rolling " << roll << ".  " << numFled << " flee.";
        onFlee(numFled);
    }

    m_currentRecord.m_numFled += numFled;

    // restore models
    if (numRestored > 0) {
        PLOG_INFO << name() << " restored " << numRestored << " models during battleshock.";
        restoreModels(numRestored);
    }

    if (remainingModels() == 0) {
        PLOG_INFO << name() << " is slain in battleshock phase.";
        onFriendlyUnitSlain(nullptr);
    }

    return numFled;
}

Unit::Unit(std::string name, int move, int wounds, int bravery, int save, bool fly, int points) :
        m_name(std::move(name)),
        m_move(move),
        m_wounds(wounds),
        m_bravery(bravery),
        m_save(save),
        m_points(points),
        m_fly(fly) {
}

void Unit::beginRound(int battleRound) {
    m_battleRound = battleRound;
    onBeginRound(battleRound);
}

void Unit::endRound(int battleRound) {
    onEndRound(battleRound);
}

void Unit::beginTurn(int battleRound, PlayerId playerWithTurn) {
    m_ran = false;
    m_charged = false;
    m_moved = false;
    m_hasFought = false;
    m_modelsSlain = 0;
    m_spellsCast = 0;
    m_prayersAttempted = 0;

    m_currentRecord.clear();

    m_currentRecord.m_playerWithTurn = playerWithTurn;
    m_currentRecord.m_round = battleRound;

    onBeginTurn(battleRound, playerWithTurn);
}

void Unit::endTurn(int battleRound) {
    onEndTurn(battleRound, m_currentRecord.m_playerWithTurn);
    m_statistics.record(m_currentRecord);
}

void Unit::addModel(Model *model) {
    m_models.push_back(std::unique_ptr<Model>(model));
    m_basesizeMm = (double) model->basesize();
}

int Unit::applyDamage(const Wounds &totalWoundsInflicted, Unit *attackingUnit) {
    // apply wound/mortal wound save
    auto totalWounds = applyWoundSave(totalWoundsInflicted, attackingUnit);

    if (!m_abilityBuffs[Ability::Ignore_All_Wounds_On_Value].empty()) {
        const auto value = m_abilityBuffs[Ability::Ignore_All_Wounds_On_Value].front().value;
        Dice::RollResult woundSaves, mortalSaves;
        Dice::RollD6(totalWounds.normal, woundSaves);
        Dice::RollD6(totalWounds.mortal, mortalSaves);

        totalWounds.normal -= woundSaves.rollsGE(value);
        totalWounds.mortal -= mortalSaves.rollsGE(value);
        totalWounds.clamp();
    }

    // apply global wound save(s)
    // TOOD: allow more than one active global wound save by accumulating the wounds
    // TODO: FIXME: always returning 0 wounds.
    //totalWounds = s_globalWoundSave(totalWounds, this, attackingUnit);

    // apply both normal and mortal wounds
    int totalDamage = totalWounds.normal + totalWounds.mortal;
    int numSlain = 0;
    for (const auto &model : m_models) {
        if (model->slain() || model->fled()) continue;

        auto wounds = model->woundsRemaining();
        auto remainingWounds = model->applyWound(totalDamage);
        if (totalDamage >= wounds) {
            totalDamage -= wounds;
        } else {
            totalDamage = 0;
        }
        if (remainingWounds == 0) {
            onFriendlyModelSlain(1, attackingUnit, totalWounds.source);
            numSlain++;
        }
    }

    onWounded();
    m_modelsSlain += numSlain;

    m_currentRecord.m_modelsSlain += numSlain;
    m_currentRecord.m_woundsTaken += totalWounds;

    if (remainingModels() == 0) {
        onFriendlyUnitSlain(attackingUnit);
        attackingUnit->onEnemyUnitSlain(this);
    }

    return numSlain;
}

int Unit::remainingModels() const {
    int models = 0;
    for (const auto &m : m_models) {
        if (!m->slain() && !m->fled())
            models++;
    }
    return models;
}

int Unit::remainingWounds() const {
    int wounds = 0;
    for (const auto &m : m_models) {
        if (!m->slain() && !m->fled())
            wounds += m->woundsRemaining();
    }
    return wounds;
}

bool Unit::hasKeyword(Keyword word) const {
    auto kw = std::find(m_keywords.begin(), m_keywords.end(), word);
    return (kw != m_keywords.end());
}

bool Unit::addKeyword(Keyword word) {
    m_keywords.push_back(word);
    return true;
}

void Unit::removeKeyword(Keyword word) {
    auto kw = std::find(m_keywords.begin(), m_keywords.end(), word);
    if (kw != m_keywords.end()) {
        m_keywords.erase(kw);
    }
}

int Unit::braveryModifier() const {
    int modifier = remainingModels() / 10 + s_globalBraveryMod(this, accumulate);
    if (m_attributeModifiers.contains(Attribute::Bravery)) {
        for (auto &bi : m_attributeModifiers.at(Attribute::Bravery)) {
            modifier += bi.modifier;
        }
    }
    return modifier;
}

void Unit::restore() {
    for (auto &m : m_models) {
        m->restore();
    }

    m_battleRound = 0;
    m_modelsSlain = 0;
    m_ran = false;
    m_charged = false;
    m_moved = false;
    m_canMove = true;

    m_spellsCast = 0;
    m_spellsUnbound = 0;
    m_prayersAttempted = 0;

    m_currentRecord.clear();

    for (auto list : m_attributeModifiers) {
        list.second.clear();
    }
    for (auto list : m_rollModifiers) {
        list.second.clear();
    }
    for (auto list : m_movementRules) {
        list.second.clear();
    }

    onRestore();
}

void Unit::deploy(const Math::Point3 &pos, const Math::Vector3 &orientation) {

    formation(computeFormation());

    m_position = pos;

    const auto modelSpacing = std::min(1.0, basesizeInches() * 0.3);
    int unitW = (int) m_models.size() / m_ranks;
    double unitWidth = m_models.size() * (basesizeInches() + modelSpacing) / (double) m_ranks;

    Math::Vector3 up(0, 0, 1);
    Math::Vector3 left = Math::Vector3::Cross(up, orientation);
    left.normalize();

    int xi = 0;
    int yi = 0;

    Math::Vector3 dx = -left * (basesizeInches() + modelSpacing);
    Math::Vector3 dy = -orientation * (basesizeInches() + modelSpacing);

    Math::Vector3 pos0 = Math::Vector3(pos.x, pos.y, pos.z) + left * unitWidth / 2.0;
    Math::Vector3 curPos = pos0;

    // Pack models into block of [numModels / m_ranks, m_ranks]
    for (auto &m : m_models) {
        m->setPosition(Math::Point3(curPos.x, curPos.y, curPos.z));
        curPos += dx;
        xi++;
        if (xi >= unitW) {
            xi = 0;
            yi++;

            curPos = pos0;
            curPos += (double) yi * dy;
        }
    }
}

bool Unit::move(const Math::Point3 &pos, const Math::Vector3 &orientation) {
    m_position = pos;

    const auto modelSpacing = std::min(1.0, basesizeInches() * 0.3);
    int unitW = (int) m_models.size() / m_ranks;
    double unitWidth = m_models.size() * (basesizeInches() + modelSpacing) / (double) m_ranks;

    Math::Vector3 up(0, 0, 1);
    Math::Vector3 left = Math::Vector3::Cross(up, orientation);
    left.normalize();

    int xi = 0;
    int yi = 0;

    Math::Vector3 dx = -left * (basesizeInches() + modelSpacing);
    Math::Vector3 dy = -orientation * (basesizeInches() + modelSpacing);

    Math::Vector3 pos0 = Math::Vector3(pos.x, pos.y, pos.z) + left * unitWidth / 2.0;
    Math::Vector3 curPos = pos0;

    // Move models as a pack of [numModels / m_ranks, m_ranks]
    for (auto &m : m_models) {
        Board::Instance()->moveModel(*m, Math::Point3(curPos.x, curPos.y, curPos.z));

        xi++;
        curPos += dx;
        if (xi >= unitW) {
            xi = 0;
            yi++;

            curPos = pos0;
            curPos += (double) yi * dy;
        }
    }

    return true;
}

bool Unit::formation(int ranks) {
    if (ranks < 1 || ranks > (int) m_models.size())
        return false;

    m_ranks = ranks;
    return true;
}

double Unit::distanceTo(const Unit *unit) const {
    if (unit == nullptr || unit->remainingModels() == 0 || remainingModels() == 0)
        return 0;

    const auto pos = position();
    const auto unitPos = unit->position();

    double dist = pos.distance(unitPos);
    return std::max(0.0, dist - (basesizeInches() + unit->basesizeInches()) / 2.0);
}

double Unit::distanceTo(const Math::Point3 &point) const {
    const auto pos = position();

    double dist = pos.distance(point);
    return std::max(0.0, dist - basesizeInches() / 2.0);
}

double Unit::distanceBetween(const Model *model, const Unit *unit) const {
    if (model == nullptr || unit == nullptr || unit->remainingModels() == 0)
        return 0;

    const double tbs = unit->basesizeInches() / 2;

    const double x = model->x();
    const double y = model->y();

    auto minDist = DBL_MAX;

    // find closes model in target unit
    for (auto &m : unit->m_models) {
        const double tx = m->x();
        const double ty = m->y();

        const double dx = fabs(tx - x);
        const double dy = fabs(ty - y);

        auto dist = sqrt(dx * dx + dy * dy) - (basesizeInches() / 2 + tbs);
        if (dist < minDist) {
            minDist = dist;
        }
    }

    return std::max(0.0, minDist);
}

void Unit::hero(PlayerId player) {
    timeoutBuffs(GamePhase::Hero, player);

    onStartHero(player);

    if (player == owningPlayer()) {
        // Use command ability.
        useCommandAbility();

        // Cast spell
        castSpell();

        // Make prayer
        makePrayer();
    } else {
        // Opposing player's hero phase
    }
}

void Unit::movement(PlayerId player) {
    timeoutBuffs(GamePhase::Movement, player);

    onStartMovement(player);

    m_moved = false;

    // Unit cannot move
    if (!canMove()) return;

    auto closestTarget = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));

    if (closestTarget) {
        auto distance = distanceTo(closestTarget);
        double totalMoveDistance = 0;

        double allowedMove = move();
        if (!m_movementRules[MovementRule::Halve_Movement].empty()) {
            if (m_movementRules[MovementRule::Halve_Movement].front().allowed) {
                allowedMove = allowedMove / 2;
            }
        }
        if (!m_movementRules[MovementRule::Double_Movement].empty()) {
            if (m_movementRules[MovementRule::Double_Movement].front().allowed) {
                allowedMove = allowedMove * 2;
            }
        }
        if (!m_movementRules[MovementRule::Triple_Movement].empty()) {
            if (m_movementRules[MovementRule::Triple_Movement].front().allowed) {
                allowedMove = allowedMove * 3;
            }
        }

        const auto movement = allowedMove + (double) moveModifier();

        double maxRange = 0.0;
        if (hasShootingAttack(maxRange)) {

            // get into range (run or not?)
            if (distance > maxRange + movement) {
                // too far to get into range with normal move - run
                auto runDist = rollRunDistance();
                if ((runDist < 3) && (runRerolls() != Rerolls::None))
                    runDist = rollRunDistance();
                totalMoveDistance = movement + runDist;
                m_ran = true;

                m_currentRecord.m_moved = movement;
                m_currentRecord.m_ran = runDist;
            } else if (distance > maxRange) {
                // move toward unit
                totalMoveDistance = movement;

                m_currentRecord.m_moved = movement;
            } else {
                // already in range - stand still
                m_currentRecord.m_moved = 0;
            }
        } else {

            if (distance <= MIN_CHARGE_DISTANCE) {
                // already in charge range - stand still
                m_currentRecord.m_moved = 0;
            }
            if ((distance > maxRange + movement)) {
                // too far to get into range with normal move - run
                auto runDist = rollRunDistance();
                if ((runDist < 3) && (runRerolls() != Rerolls::None))
                    runDist = rollRunDistance();
                totalMoveDistance = movement + runDist;
                m_ran = true;

                m_currentRecord.m_moved = movement;
                m_currentRecord.m_ran = runDist;
            } else {
                // normal move toward unit
                totalMoveDistance = std::min(movement, std::max(0.0, distance - MIN_CHARGE_DISTANCE));

                m_currentRecord.m_moved = totalMoveDistance;
            }
        }
        Math::Ray ray(position(), closestTarget->position());
        auto newPos = ray.pointAt(totalMoveDistance);

        move(newPos, ray.getDirection());

        m_moved = (totalMoveDistance > 0.0);
    } else {
        // no target units - stand here confused!!!
        m_moved = false;
    }

    if (m_ran)
        onRan();

    onEndMovement(player);
}

bool Unit::battleshockRequired() const {
    if (m_abilityBuffs.contains(Ability::Ignore_Battleshock)) {
        if (m_abilityBuffs.at(Ability::Ignore_Battleshock).empty()) {
            return true;
        }
        return (m_abilityBuffs.at(Ability::Ignore_Battleshock).front().value > 0);
    }
    return true;
}

int Unit::rollBattleshock() const {
    return Dice::RollD6();
}

Wounds Unit::shoot(PlayerId player, int &numSlain) {
    if (m_shootingTarget == nullptr) {
        numSlain = 0;
        return {0, 0};
    }
    auto wounds = shoot(-1, m_shootingTarget, numSlain);

    onEndShooting(player);

    return wounds;
}

Wounds Unit::fight(PlayerId player, int &numSlain) {
    if (hasFought() || (m_meleeTarget == nullptr) || m_meleeTarget->remainingModels() == 0) {
        numSlain = 0;
        return {0, 0};
    }
    auto wounds = fight(-1, m_meleeTarget, numSlain);

    onEndCombat(player);

    return wounds;
}

void Unit::shooting(PlayerId player) {
    timeoutBuffs(GamePhase::Shooting, player);

    // TODO: unit that is currently in combat must shoot at the combat unit
    auto enemyUnits = Board::Instance()->getAllUnits(GetEnemyId(player));
    m_shootingTarget = nullptr;
    double closestDistance = DBL_MAX;
    for (auto unit : enemyUnits) {
        if (unit->remainingModels() <= 0) continue;

        if (distanceTo(unit) < closestDistance) {
            m_shootingTarget = unit;
            closestDistance = distanceTo(unit);
        }
    }

    onStartShooting(player);
}

void Unit::combat(PlayerId player) {
    timeoutBuffs(GamePhase::Combat, player);

    auto enemyUnits = Board::Instance()->getAllUnits(GetEnemyId(player));
    m_meleeTarget = nullptr;
    double closestDistance = DBL_MAX;
    for (auto unit : enemyUnits) {
        if (unit->remainingModels() <= 0) continue;

        if (distanceTo(unit) < closestDistance) {
            m_meleeTarget = unit;
            closestDistance = distanceTo(unit);
        }
    }

    onStartCombat(player);
}

void Unit::charge(PlayerId player) {
    timeoutBuffs(GamePhase::Charge, player);

    auto board = Board::Instance();

    auto weapon = m_models.front()->preferredWeapon();
    assert(weapon);
    if (weapon && weapon->isMissile()) {
        // Shooty unit does not want to charge
        onEndCharge(player);
        return;
    }

    auto otherRoster = board->getPlayerRoster(GetEnemyId(player));
    auto closestTarget = otherRoster ? otherRoster->nearestUnit(this) : nullptr;

    if (m_ran && !canRunAndCharge()) {
        // Can't run and charge.
        onEndCharge(player);
        return;
    }

    if (closestTarget) {
        auto distance = distanceTo(closestTarget);

        if ((distance <= m_maxChargeDistance) && (distance >= MIN_CHARGE_DISTANCE)) {
            auto chargeDist = (double) rollChargeDistance();
            if (chargeDist >= distance) {
                m_charged = true;

                chargeDist = distance;

                Math::Ray ray(position(), closestTarget->position());
                auto newPos = ray.pointAt(chargeDist);
                move(newPos, ray.getDirection());
            } else if (chargeRerolls() != Rerolls::None) {
                chargeDist = (double) rollChargeDistance();
                if (chargeDist >= distance) {
                    m_charged = true;

                    chargeDist = distance;

                    Math::Ray ray(position(), closestTarget->position());
                    auto newPos = ray.pointAt(chargeDist);
                    move(newPos, ray.getDirection());
                }
            }

            m_currentRecord.m_charged = chargeDist;
        } else {
            // too far to charge
            m_currentRecord.m_charged = 0;
        }
    } else {
        // no target units - stand here confused!!!
        m_currentRecord.m_charged = 0;
    }

    if (m_charged)
        onCharged();

    onEndCharge(player);
}

void Unit::battleshock(PlayerId player) {

    timeoutBuffs(GamePhase::Battleshock, player);

    onStartBattleshock(player);

    int numFleeing = applyBattleshock();
    if (numFleeing > 0) {
        PLOG_INFO << "A total of " << numFleeing << " " << name() << " from " << PlayerIdToString(owningPlayer())
                  << " due to battleshock.";
    }

    onEndBattleshock(player);
}

int Unit::rollChargeDistance() {
    // TODO: Split charge roll from charge modifications.
    m_unmodifiedChargeRoll = Dice::Roll2D6();
    if (!m_movementRules[MovementRule::Halve_Charge_Roll].empty()) {
        if (m_movementRules[MovementRule::Halve_Charge_Roll].front().allowed) {
            m_unmodifiedChargeRoll = (m_unmodifiedChargeRoll + 1) / 2; // Round up
        }
    }
    return m_unmodifiedChargeRoll + chargeModifier();
}

int Unit::rollRunDistance() {
    int roll = Dice::RollD6();
    if (m_abilityBuffs[Ability::Auto_Max_Run].empty()) {
        roll = 6;
    }
    if (!m_movementRules[MovementRule::Halve_Run_Roll].empty()) {
        if (m_movementRules[MovementRule::Halve_Run_Roll].front().allowed) {
            roll = (roll + 1) / 2; // Round up
        }
    }
    return roll + runModifier();
}

int Unit::slay(int numModels) {
    if (numModels <= 0)
        return 0;

    int numSlain = 0;
    for (auto &model : m_models) {
        if (model->slain() || model->fled()) continue;

        model->slay();
        numSlain++;

        if (numSlain >= numModels) break;
    }
    return numSlain;
}

int Unit::heal(int numWounds) {
    if (numWounds <= 0)
        return 0;

    int numHealedWounds = 0;
    for (auto &model : m_models) {
        if (model->slain() || model->fled()) continue;

        auto toHeal = wounds() - model->woundsRemaining();
        model->applyHealing(toHeal);
        if (toHeal >= numWounds) {
            numHealedWounds += numWounds;
            break;
        } else {
            numWounds -= toHeal;
            numHealedWounds += toHeal;
        }
    }
    return numHealedWounds;
}

bool Unit::makeSave(const Weapon *weapon, int weaponRend, Unit *attacker, int &saveRoll) {
    auto saveModifiers = toSaveModifier(weapon, attacker) + targetSaveModifier(weapon, attacker);
    auto effectiveRend = m_ignoreRend ? 0 : weaponRend;
    auto toSave = m_save + saveModifiers - effectiveRend;

    saveRoll = Dice::RollD6();
    if (saveRoll < toSave) {
        auto reroll = toSaveRerolls(weapon, attacker);
        if (reroll == Rerolls::Failed) {
            saveRoll = Dice::RollD6();
        } else if ((reroll == Rerolls::Ones) && (saveRoll == 1)) {
            saveRoll = Dice::RollD6();
        } else if ((reroll == Rerolls::Ones_And_Twos) && (saveRoll == 1 || saveRoll == 2)) {
            saveRoll = Dice::RollD6();
        }
    }

    const bool saveMade = (saveRoll >= toSave);

    if (saveMade)
        m_currentRecord.m_savesMade++;
    else
        m_currentRecord.m_savesFailed++;

    return saveMade;
}

void Unit::attackWithWeapon(const Weapon *weapon, Unit *target, const Model *fromModel,
                            Wounds &totalWoundsInflicted, Wounds &totalWoundsSuffered) {
    totalWoundsInflicted = {0, 0, weapon->isMissile() ? Wounds::Source::Weapon_Missile : Wounds::Source::Weapon_Melee};
    totalWoundsSuffered = {0, 0};

    if (!weapon->isActive()) {
        // not active
        return;
    }

    // check range to target unit
    double distanceToTarget = distanceBetween(fromModel, target);
    if (distanceToTarget > (double) weapon->range()) {
        // out of range
        return;
    }

    const auto rerolls = toHitRerolls(weapon, target);
    const auto woundRerolls = toWoundRerolls(weapon, target);
    const int totalHitModifiers = toHitModifier(weapon, target) + target->targetHitModifier(weapon, this);
    const int totalWoundModifiers = toWoundModifier(weapon, target) + target->targetWoundModifier(weapon, this);

    /*
    auto hits = weapon->rollToHit(totalHitModifiers, rerolls, extraAttacks(fromModel, weapon, target));
    m_currentRecord.m_attacksMade += hits.totalAttacks;
    m_currentRecord.m_attacksHitting += hits.numHits;

    auto woundingHits = weapon->rollToWound(hits.numHits, totalWoundModifiers, woundRerolls);
    m_currentRecord.m_attacksWounding += woundingHits.numWoundingHit;
    for (auto h = 0; h < woundingHits.numWoundingHit; h++) {
        // roll save
        int saveRoll = 0;
        auto rend = weaponRend(weapon, target, hitRoll, woundRoll);
        if (weapon->isMissile()) {
            for (auto ri : m_attributeModifiers[Attribute::Weapon_Rend_Missile]) {
                rend += ri.modifier;
            }
        } else {
            for (auto ri : m_attributeModifiers[Attribute::Weapon_Rend_Melee]) {
                rend += ri.modifier;
            }
        }

    }
    */

    const int totalAttacks = weapon->numAttacks(extraAttacks(fromModel, weapon, target));
    for (auto a = 0; a < totalAttacks; a++) {
        m_currentRecord.m_attacksMade++;

        // roll to hit
        auto hitRoll = Dice::RollD6();
        auto modifiedHitRoll = hitRoll + totalHitModifiers;
        // TODO: clarify re-rolling - after or before modifiers?
        if (modifiedHitRoll < weapon->toHit()) {
            hitRoll = rerolling(hitRoll, rerolls);
            modifiedHitRoll = hitRoll + totalHitModifiers;
        }

        if (modifiedHitRoll >= weapon->toHit()) {
            // apply hit modifiers (a single hit may result in multiple hits)
            int numHits = generateHits(hitRoll, weapon, target);
            if (!m_abilityBuffs[Ability::Extra_Hit_On_Value].empty()) {
                const int value = m_abilityBuffs[Ability::Extra_Hit_On_Value].front().value;
                if (hitRoll >= value) numHits++;
            }
            for (auto h = 0; h < numHits; h++) {
                // roll to wound
                auto woundRoll = Dice::RollD6();
                auto modifiedWoundRoll = woundRoll + totalWoundModifiers;
                if (modifiedWoundRoll < weapon->toWound()) {
                    woundRoll = rerolling(woundRoll, woundRerolls);
                    modifiedWoundRoll = woundRoll + totalWoundModifiers;
                }

                if (modifiedWoundRoll >= weapon->toWound()) {
                    m_currentRecord.m_attacksHitting++;

                    // roll save
                    int saveRoll = 0;
                    auto rend = weaponRend(fromModel, weapon, target, hitRoll, woundRoll);
                    if (weapon->isMissile()) {
                        for (auto ri : m_attributeModifiers[Attribute::Weapon_Rend_Missile]) {
                            rend += ri.modifier;
                        }
                    } else {
                        for (auto ri : m_attributeModifiers[Attribute::Weapon_Rend_Melee]) {
                            rend += ri.modifier;
                        }
                    }

                    if (!target->makeSave(weapon, rend, this, saveRoll)) {
                        // compute damage
                        auto dam = weaponDamage(fromModel, weapon, target, hitRoll, woundRoll);

                        if (!m_abilityBuffs[Ability::Extra_Mortal_Wound_On_Hit_Roll].empty()) {
                            const int value = m_abilityBuffs[Ability::Extra_Mortal_Wound_On_Hit_Roll].front().value;
                            if (hitRoll >= value)
                                dam.mortal++;
                        }

                        if (weapon->isMissile()) {
                            for (auto ri : m_attributeModifiers[Attribute::Weapon_Damage_Missile]) {
                                dam.normal += ri.modifier;
                            }
                        } else {
                            for (auto ri : m_attributeModifiers[Attribute::Weapon_Damage_Melee]) {
                                dam.normal += ri.modifier;
                            }
                        }

                        // modify damage
                        dam = target->targetAttackDamageModifier(dam, this, hitRoll, woundRoll);

                        PLOG_INFO <<"\tWeapon, " << weapon->name() << ", inflicted wounds " << dam << " on " << target->name();

                        totalWoundsInflicted += dam;
                    } else {
                        // made save
                        PLOG_INFO << "\t" << target->name() << " made a save again weapon " << weapon->name() << " rolling a " << saveRoll;
                    }

                    totalWoundsSuffered += target->computeReturnedDamage(weapon, saveRoll);
                } else {
                    // failed to wound
                    PLOG_INFO << "\tWeapon, " << weapon->name() << ", failed to wound rolling a " << modifiedWoundRoll;
                }
            }
        } else {
            // missed
            PLOG_INFO << "\tWeapon, " << weapon->name() << ", missed with a roll of " << modifiedHitRoll;
        }
    }

}

int Unit::rerolling(int initialRoll, Rerolls reroll) const {
    int roll = initialRoll;
    if (reroll == Rerolls::Failed) {
        roll = Dice::RollD6();
    }
    if ((reroll == Rerolls::Ones) && (initialRoll == 1)) {
        roll = Dice::RollD6();
    }
    if ((reroll == Rerolls::Ones_And_Twos) && ((initialRoll == 1) || (initialRoll == 2))) {
        roll = Dice::RollD6();
    }
    return roll;
}

void Unit::computeBattleshockEffect(int roll, int &numFled, int &numRestored) const {
    auto mod = s_globalBattleshockFleeModifier(this, roll);
    numFled = (m_modelsSlain + roll) - (m_bravery + braveryModifier()) + mod;
    numFled = std::max(0, std::min(remainingModels(), numFled));
    numRestored = 0;
}

int Unit::remainingPoints() const {
    auto pointsPerWound = (double) m_points / (double) (m_models.size() * wounds());
    auto points = (int) round(pointsPerWound * (double) remainingWounds());
    return points;
}

bool Unit::unbind(Unit *caster, int castRoll) {
    bool unbound = false;

    if (distanceTo(caster) <= unbindingDistance()) {
        if (m_spellsUnbound < m_totalUnbinds) {
            int unbindRoll = Dice::Roll2D6() + unbindingModifier();
            if (unbindRoll > castRoll) {
                PLOG_INFO.printf("%s unbound a spell cast by %s (%d) with a unbind roll of %d.",
                                 name().c_str(), caster->name().c_str(), castRoll, unbindRoll);
                unbound = true;
                onUnboundSpell(caster, castRoll);
            } else {
                PLOG_INFO.printf("%s failed to unbind a spell cast by %s (%d) with a unbind roll of %d.",
                                 name().c_str(), caster->name().c_str(), castRoll, unbindRoll);
            }
            m_spellsUnbound++;
        }
    }
    return unbound;
}

void Unit::castSpell() {
    if (m_spellsCast < m_totalSpells) {
        // TODO: Allow unit to cast multiple spells.  A given spell can only be cast once per player per hero phase.

        for (auto &sip : m_knownSpells) {
            if (sip == nullptr) continue;

            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), sip->range());
            if (sip->allowedTargets() == Abilities::Target::Friendly) {
                units = Board::Instance()->getUnitsWithin(this, owningPlayer(), sip->range());
                units.push_back(this);
            }

            // TODO: determine 'best' spell to cast on the 'best' target.

            // cast the first spell on the first unit
            for (auto ip : units) {
                auto successful = sip->cast(ip, m_battleRound);
                if (successful == Spell::Result::Success) {
                    PLOG_INFO.printf("%s successfully cast %s", m_name.c_str(), sip->name().c_str());
                    onCastSpell(sip.get(), ip);
                }
                m_spellsCast++;
                return;
            }
        }
    }
}

void Unit::useCommandAbility() {
    int cpAvailable = getCommandPoints();
    if (cpAvailable) {
        // Select _best_ command ability to use (if any)
        AbilityTarget target;
        CommandAbility *cip = SelectCommandAbility(this, target);
        if (cip) {
            if (target.m_targetUnit)
                cip->use(target.m_target, m_battleRound);
            else
                cip->use(target.m_x, target.m_y, m_battleRound);
            onCommandAbilityUsed(cip, target.m_target);
        }
    }
}

void Unit::makePrayer() {
    if (m_prayersAttempted < m_totalPrayers) {
        // TODO: Allow unit to attempt multiple prayers.  A given prayer can only be attempted once per player per hero phase.
        for (auto &sip : m_knownPrayers) {
            if (sip == nullptr) continue;

            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), sip->range());
            if (sip->allowedTargets() == Abilities::Target::Friendly) {
                units = Board::Instance()->getUnitsWithin(this, owningPlayer(), sip->range());
                units.push_back(this);
            }

            // TODO: determine 'best' prayer to attempt on the 'best' target.

            // attempt the first prayer on the first unit
            for (auto ip : units) {
                bool successful = sip->pray(ip, m_battleRound);
                if (successful) {
                    onPrayerMade(sip.get(), ip);
                    PLOG_INFO.printf("%s successfully attempted %s", m_name.c_str(), sip->name().c_str());
                }
                m_prayersAttempted++;
                return;
            }
        }
    }
}

bool Unit::buffModifier(Attribute which, int modifier, Duration duration) {
    ModifierBuff buff = {modifier, duration};
    m_attributeModifiers[which].push_back(buff);

    return true;
}

bool Unit::buffReroll(Attribute which, Rerolls reroll, Duration duration) {
    RerollBuff buff = {reroll, duration};
    m_rollModifiers[which].push_back(buff);

    return true;
}

bool Unit::buffMovement(MovementRule which, bool allowed, Duration duration) {
    MovementRuleBuff buff = {allowed, duration};
    m_movementRules[which].push_back(buff);

    return true;
}

bool Unit::buffAbility(Ability which, int value, Duration duration) {
    AbilityBuff buff = {value, duration};
    m_abilityBuffs[which].push_back(buff);

    return true;
}

const Model *Unit::nearestModel(const Model *model, const Unit *targetUnit) const {
    if (!targetUnit || targetUnit->m_models.empty()) { return nullptr; }

    auto nearestModel = targetUnit->m_models[0].get();
    auto minDistance = Math::g_infinite;
    for (const auto &m : targetUnit->m_models) {
        const auto dist = Model::DistanceBetween(m.get(), model);
        if (dist < minDistance) {
            minDistance = dist;
            nearestModel = m.get();
        }
    }
    return nearestModel;
}

void Unit::doPileIn() {
    if (!m_meleeTarget) return;

    if (!m_movementRules[MovementRule::Can_PileIn].empty()) {
        if (!m_movementRules[MovementRule::Can_PileIn].front().allowed) {
            return;
        }
    }

    auto pileInMove = m_pileInMove;
    for (auto pi : m_attributeModifiers[Attribute::Pile_In_Distance]) {
        pileInMove += pi.modifier;
    }

    // Pile in up to 3" towards nearest enemy model.
    for (auto &m : m_models) {
        // Find closest model in melee target unit.
        auto closestTarget = nearestModel(m.get(), m_meleeTarget);
        // Move toward that model if possible
        if (closestTarget) {
            auto totalMoveDistance = std::min((double) pileInMove, Model::DistanceBetween(m.get(), closestTarget));
            const Math::Ray ray(m->position(), closestTarget->position());
            auto newPos = ray.pointAt(totalMoveDistance);
            m->setPosition(newPos);
            //Board::Instance()->moveModel(*m, newPos);
        }
    }
}

int Unit::numModelsWithin(const Model *model, double range) const {
    int count = 0;
    // Count the number of remaining models from this unit within 'range' of the given model.
    for (auto &m : m_models) {
        if (!m->slain() && !m->fled()) {
            double distanceToModel = Model::DistanceBetween(m.get(), model);
            if (distanceToModel <= range) count++;
        }
    }
    return count;
}


void Unit::timeoutBuffs(GamePhase phase, PlayerId player) {
    Duration currentPhase = {phase, m_battleRound, player};

    // Remove all buffs that expire in the player's given phase
    for (auto &list : m_attributeModifiers) {
        for (auto bi = list.second.begin(); bi != list.second.end();) {
            if (Expired(bi->duration, currentPhase)) {
                bi = list.second.erase(bi);
            } else {
                ++bi;
            }
        }
    }

    for (auto &list : m_rollModifiers) {
        for (auto bi = list.second.begin(); bi != list.second.end();) {
            if (Expired(bi->duration, currentPhase)) {
                bi = list.second.erase(bi);
            } else {
                ++bi;
            }
        }
    }

    for (auto &list : m_movementRules) {
        for (auto bi = list.second.begin(); bi != list.second.end();) {
            if (Expired(bi->duration, currentPhase)) {
                bi = list.second.erase(bi);
            } else {
                ++bi;
            }
        }
    }

    for (auto &list : m_abilityBuffs) {
        for (auto bi = list.second.begin(); bi != list.second.end();) {
            if (Expired(bi->duration, currentPhase)) {
                bi = list.second.erase(bi);
            } else {
                ++bi;
            }
        }
    }
}

int Unit::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
    int extra = s_globalAttackMod(this, attackingModel, weapon, target, accumulate);

    Attribute which = Attribute::Attacks_Melee;
    if (weapon->isMissile())
        which = Attribute::Attacks_Missile;

    if (m_attributeModifiers.contains(which)) {
        for (auto bi : m_attributeModifiers.at(which)) {
            extra += bi.modifier;
        }
    }
    extra += UnitModifierInterface::extraAttacks(attackingModel, weapon, target);

    return extra;
}

int Unit::toHitModifier(const Weapon *weapon, const Unit *target) const {
    int modifier = s_globalToHitMod(this, weapon, target, accumulate);

    Attribute which = Attribute::To_Hit_Melee;
    if (weapon->isMissile())
        which = Attribute::To_Hit_Missile;

    if (m_attributeModifiers.contains(which)) {
        for (auto bi : m_attributeModifiers.at(which)) {
            modifier += bi.modifier;
        }
    }
    modifier += UnitModifierInterface::toHitModifier(weapon, target);

    return modifier;
}

int Unit::toWoundModifier(const Weapon *weapon, const Unit *target) const {
    int modifier = s_globalToWoundMod(this, weapon, target, accumulate);

    Attribute which = Attribute::To_Wound_Melee;
    if (weapon->isMissile())
        which = Attribute::To_Wound_Missile;

    if (m_attributeModifiers.contains(which)) {
        for (auto bi : m_attributeModifiers.at(which)) {
            modifier += bi.modifier;
        }
    }
    modifier += UnitModifierInterface::toWoundModifier(weapon, target);

    return modifier;
}

Rerolls Unit::toHitRerolls(const Weapon *weapon, const Unit *target) const {
    Attribute which = Attribute::To_Hit_Melee;
    if (weapon->isMissile())
        which = Attribute::To_Hit_Missile;

    auto globalRR = s_globalToHitReroll(this, weapon, target);
    if (globalRR != Rerolls::None)
        return globalRR;

    auto baseRR = UnitModifierInterface::toHitRerolls(weapon, target);
    if (baseRR != Rerolls::None)
        return baseRR;

    if (m_rollModifiers.contains(which)) {
        if (m_rollModifiers.at(which).empty())
            return Rerolls::None;
        return m_rollModifiers.at(which).front().rerolls;
    }
    return Rerolls::None;
}

Rerolls Unit::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
    Attribute which = Attribute::To_Wound_Melee;
    if (weapon->isMissile())
        which = Attribute::To_Wound_Missile;

    auto globalRR = s_globalToWoundReroll(this, weapon, target);
    if (globalRR != Rerolls::None)
        return globalRR;

    auto baseRR = UnitModifierInterface::toWoundRerolls(weapon, target);
    if (baseRR != Rerolls::None)
        return baseRR;

    if (m_rollModifiers.contains(which)) {
        if (m_rollModifiers.at(which).empty())
            return Rerolls::None;
        return m_rollModifiers.at(which).front().rerolls;
    }
    return Rerolls::None;
}

int Unit::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
    int modifier = s_globalSaveMod(this, weapon, accumulate);

    auto which = Attribute::To_Save_Missile;
    if (weapon->isMelee())
        which = Attribute::To_Save_Melee;

    if (m_attributeModifiers.contains(which)) {
        for (auto bi : m_attributeModifiers.at(which)) {
            modifier += bi.modifier;
        }
    }
    modifier += UnitModifierInterface::toSaveModifier(weapon, attacker);

    return modifier;
}

Rerolls Unit::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
    auto globalRR = s_globalSaveReroll(this, weapon, attacker);
    if (globalRR != Rerolls::None)
        return globalRR;

    auto baseRR = UnitModifierInterface::toSaveRerolls(weapon, attacker);
    if (baseRR != Rerolls::None)
        return baseRR;

    auto which = Attribute::To_Save_Missile;
    if (weapon->isMelee())
        which = Attribute::To_Save_Melee;

    if (m_rollModifiers.contains(which)) {
        if (m_rollModifiers.at(which).empty())
            return Rerolls::None;
        return m_rollModifiers.at(which).front().rerolls;
    }
    return Rerolls::None;
}

Rerolls Unit::battleshockRerolls() const {
    auto globalRR = s_globalBattleshockReroll(this);
    if (globalRR != Rerolls::None)
        return globalRR;

    auto baseRR = UnitModifierInterface::battleshockRerolls();
    if (baseRR != Rerolls::None)
        return baseRR;

    if (m_rollModifiers.contains(Attribute::Bravery)) {
        if (m_rollModifiers.at(Attribute::Bravery).empty())
            return Rerolls::None;
        return m_rollModifiers.at(Attribute::Bravery).front().rerolls;
    }
    return Rerolls::None;
}

int Unit::castingModifier() const {
    int modifier = s_globalCastMod(this, accumulate);
    if (m_attributeModifiers.contains(Attribute::Casting_Roll)) {
        for (auto bi : m_attributeModifiers.at(Attribute::Casting_Roll)) {
            modifier += bi.modifier;
        }
    }

    modifier += UnitModifierInterface::castingModifier();

    return modifier;
}

int Unit::unbindingModifier() const {
    int modifier = s_globalCastMod(this, accumulate);
    if (m_attributeModifiers.contains(Attribute::Unbinding_Roll)) {
        for (auto bi : m_attributeModifiers.at(Attribute::Unbinding_Roll)) {
            modifier += bi.modifier;
        }
    }
    modifier += UnitModifierInterface::unbindingModifier();

    return modifier;
}

int Unit::moveModifier() const {
    int modifier = s_globalMoveMod(this, accumulate);
    if (m_attributeModifiers.contains(Attribute::Move_Distance)) {
        for (auto bi : m_attributeModifiers.at(Attribute::Move_Distance)) {
            modifier += bi.modifier;
        }
    }

    modifier += UnitModifierInterface::moveModifier();

    return modifier;
}

int Unit::runModifier() const {
    int modifier = s_globalRunMod(this, accumulate);
    if (m_attributeModifiers.contains(Attribute::Run_Distance)) {
        for (auto bi : m_attributeModifiers.at(Attribute::Run_Distance)) {
            modifier += bi.modifier;
        }
    }

    modifier += UnitModifierInterface::runModifier();

    return modifier;
}

int Unit::chargeModifier() const {
    int modifier = s_globalChargeMod(this, accumulate);
    if (m_attributeModifiers.contains(Attribute::Charge_Distance)) {
        for (auto bi : m_attributeModifiers.at(Attribute::Charge_Distance)) {
            modifier += bi.modifier;
        }
    }

    modifier += UnitModifierInterface::chargeModifier();

    return modifier;
}

Rerolls Unit::runRerolls() const {
    auto globalRR = s_globalRunReroll(this);
    if (globalRR != Rerolls::None)
        return globalRR;

    auto baseRR = UnitModifierInterface::runRerolls();
    if (baseRR != Rerolls::None)
        return baseRR;

    if (m_rollModifiers.contains(Attribute::Run_Distance)) {
        if (m_rollModifiers.at(Attribute::Run_Distance).empty())
            return Rerolls::None;
        return m_rollModifiers.at(Attribute::Run_Distance).front().rerolls;
    }
    return Rerolls::None;
}

Rerolls Unit::chargeRerolls() const {
    auto globalRR = s_globalChargeReroll(this);
    if (globalRR != Rerolls::None)
        return globalRR;

    auto baseRR = UnitModifierInterface::chargeRerolls();
    if (baseRR != Rerolls::None)
        return baseRR;

    if (m_rollModifiers.contains(Attribute::Charge_Distance)) {
        if (m_rollModifiers.at(Attribute::Charge_Distance).empty())
            return Rerolls::None;
        return m_rollModifiers.at(Attribute::Charge_Distance).front().rerolls;
    }
    return Rerolls::None;
}

bool Unit::canFly() const {
    if (m_movementRules.contains(MovementRule::Can_Fly)) {
        if (m_movementRules.at(MovementRule::Can_Fly).empty())
            return m_fly;
        else
            return m_movementRules.at(MovementRule::Can_Fly).front().allowed;
    }
    return m_fly;
}

bool Unit::canMove() const {
    if (m_movementRules.contains(MovementRule::Can_Move)) {
        if (m_movementRules.at(MovementRule::Can_Move).empty())
            return m_canMove;
        else
            return m_movementRules.at(MovementRule::Can_Move).front().allowed;
    }
    return m_canMove;
}

bool Unit::canPileIn() const {
    if (m_movementRules.contains(MovementRule::Can_PileIn)) {
        if (m_movementRules.at(MovementRule::Can_PileIn).empty())
            return (m_pileInMove > 0.0);
        else
            return m_movementRules.at(MovementRule::Can_PileIn).front().allowed;
    }
    return (m_pileInMove > 0.0);
}

bool Unit::canRunAndShoot() const {
    if (m_movementRules.contains(MovementRule::Run_And_Shoot)) {
        if (m_movementRules.at(MovementRule::Run_And_Shoot).empty())
            return m_runAndShoot;
        else
            return m_movementRules.at(MovementRule::Run_And_Shoot).front().allowed;
    }
    return m_runAndShoot;
}

bool Unit::canRunAndCharge() const {
    if (m_movementRules.contains(MovementRule::Run_And_Charge)) {
        if (m_movementRules.at(MovementRule::Run_And_Charge).empty())
            return m_runAndCharge;
        else
            return m_movementRules.at(MovementRule::Run_And_Charge).front().allowed;
    }
    return m_runAndCharge;
}

bool Unit::canRetreatAndShoot() const {
    if (m_movementRules.contains(MovementRule::Retreat_And_Shoot)) {
        if (m_movementRules.at(MovementRule::Retreat_And_Shoot).empty())
            return m_retreatAndShoot;
        else
            return m_movementRules.at(MovementRule::Retreat_And_Shoot).front().allowed;
    }
    return m_retreatAndShoot;
}

bool Unit::canRetreatAndCharge() const {
    if (m_movementRules.contains(MovementRule::Retreat_And_Charge)) {
        if (m_movementRules.at(MovementRule::Retreat_And_Charge).empty())
            return m_retreatAndCharge;
        else
            return m_movementRules.at(MovementRule::Retreat_And_Charge).front().allowed;
    }
    return m_retreatAndCharge;
}

bool Unit::canUseCommandAbilities() const {
    if (m_abilityBuffs.contains(Ability::Cannot_Use_Command_Abilities)) {
        if (m_abilityBuffs.at(Ability::Cannot_Use_Command_Abilities).empty())
            return true;
        else
            return (m_abilityBuffs.at(Ability::Cannot_Use_Command_Abilities).front().value > 0);
    }
    return true;
}

int Unit::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
    int modifier = 0;

    Attribute which = Attribute::Target_To_Hit_Melee;
    if (weapon->isMissile())
        which = Attribute::Target_To_Hit_Missile;

    if (m_attributeModifiers.contains(which)) {
        for (auto &bi : m_attributeModifiers.at(which)) {
            modifier += bi.modifier;
        }
    }
    modifier += UnitModifierInterface::targetHitModifier(weapon, attacker);

    return modifier;
}

int Unit::targetWoundModifier(const Weapon *weapon, const Unit *attacker) const {
    int modifier = 0;

    Attribute which = Attribute::Target_To_Wound_Melee;
    if (weapon->isMissile())
        which = Attribute::Target_To_Wound_Missile;

    if (m_attributeModifiers.contains(which)) {
        for (auto &bi : m_attributeModifiers.at(which)) {
            modifier += bi.modifier;
        }
    }
    modifier += UnitModifierInterface::targetWoundModifier(weapon, attacker);

    return modifier;
}

int Unit::targetSaveModifier(const Weapon *weapon, const Unit *attacker) const {
    int modifier = 0;

    Attribute which = Attribute::Target_To_Save_Melee;
    if (weapon->isMissile())
        which = Attribute::Target_To_Save_Missile;

    if (m_attributeModifiers.contains(which)) {
        for (auto &bi : m_attributeModifiers.at(which)) {
            modifier += bi.modifier;
        }
    }
    modifier += UnitModifierInterface::targetSaveModifier(weapon, attacker);

    return modifier;
}

int Unit::numOfWoundedModels() const {
    int count = 0;
    for (const auto &m : m_models) {
        if (!m->slain() && !m->fled()) {
            if (m->woundsRemaining() < m->initialWounds()) {
                count++;
            }
        }
    }
    return count;
}

void Unit::visitWeapons(std::function<void(const Weapon &)> &visitor) {
    // Every unit must have at least one weapon.
    assert(!m_weapons.empty());

    for (const auto &w : m_weapons) {
        visitor(*w);
    }
}

int Unit::rollCastingWithRerolls(int castingValue, UnmodifiedCastingRoll &unmodifiedRoll) const {
    int castingRoll = rollCasting(unmodifiedRoll);
    Rerolls castingReroll = castingRerolls();
    if (castingReroll == Rerolls::None) {
        castingReroll = s_globalCastReroll(this);
    }

    if ((castingReroll == Rerolls::Successful) && (castingRoll >= castingValue)) {
        castingRoll = rollCasting(unmodifiedRoll);
    }
    else if ((castingReroll == Rerolls::Failed) && (castingRoll < castingValue)) {
        castingRoll = rollCasting(unmodifiedRoll);
    }
    return castingRoll;
}

int Unit::rollCasting(UnmodifiedCastingRoll &unmodifiedRoll) const {
    unmodifiedRoll.d1 = Dice::RollD6();
    unmodifiedRoll.d2 = Dice::RollD6();
    return unmodifiedRoll + castingModifier();
}

PlayerId Unit::owningPlayer() const {
    if (m_roster)
        return m_roster->getOwningPlayer();
    return PlayerId::None;
}

int Unit::getCommandPoints() const {
    if (m_roster)
        return m_roster->getCommandPoints();
    return 0;
}

void Unit::setRoster(Roster *roster) {
    m_roster = roster;
}

int Unit::getModelsWithin(const Model *model, const Unit *targetUnit, double distance) const {
    if (!targetUnit || targetUnit->m_models.empty()) { return 0; }

    int count = 0;
    for (const auto &m : targetUnit->m_models) {
        if (m->slain() || m->fled()) continue;

        const auto dist = Model::DistanceBetween(m.get(), model);
        if (dist < distance) {
            count++;
        }
    }
    return count;
}

int Unit::initialWounds() const {
    int wounds = 0;
    for (const auto &m : m_models) {
        wounds += m->initialWounds();
    }
    return wounds;
}

bool Unit::isFriendly(const Unit *unit) const {
    return (unit && (owningPlayer() == unit->owningPlayer()));
}

int Unit::returnModels(int numModels) {
    int numReturned = 0;

    for (auto &m : m_models) {
        if (m->slain()) {
            m->restore();
            numReturned++;
        }
        if (numReturned >= numModels) break;
    }
    return numReturned;
}

int Unit::computeFormation() const {
    // Select the formatation based on number of models, base size and weapon range
    double avgWeaponRange = 0;
    for (auto i = 0; i < numModels(); i++) {
        avgWeaponRange += getModel(i)->preferredWeapon() ? getModel(i)->preferredWeapon()->range() : 0;
    }
    avgWeaponRange /= numModels();
    auto weaponRanks = std::min(4, (int) ceil(avgWeaponRange / basesizeInches()));

    int modelsPerRank = 10;
    if (basesizeInches() <= 30) modelsPerRank = 20;
    else if (basesizeInches() >= 50) modelsPerRank = 1;
    auto numRanks = numModels() / modelsPerRank;

    return std::max(numRanks, weaponRanks);
}

bool Unit::hasShootingAttack(double &maxRange) const {
    maxRange = 0.0;
    bool found = false;
    for (auto w : m_weapons) {
        if (w->isMissile() && w->isActive() && (w->range() > maxRange)) {
            maxRange = w->range();
            found = true;
        }
    }
    return found;
}

bool Unit::isNamedModelAlive(const std::string &name) const {
    for (const auto &m : m_models) {
        if (m->getName() != name) {
            continue;
        }
        return !(m->slain() || m->fled());
    }
    return false;
}

Wounds Unit::ignoreWounds(const Wounds &wounds, int ignoreOnRoll) const {
    auto totalWounds = wounds;

    Dice::RollResult result;
    Dice::RollD6(totalWounds.normal, result);
    totalWounds.normal -= result.rollsGE(ignoreOnRoll);
    Dice::RollD6(totalWounds.mortal, result);
    totalWounds.mortal -= result.rollsGE(ignoreOnRoll);

    return totalWounds.clamp();
}

CustomUnit::CustomUnit(const std::string &name, int move, int wounds, int bravery, int save, bool fly, int points) :
        Unit(name, move, wounds, bravery, save, fly, points) {
}
