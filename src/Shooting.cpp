/*
 * Warhammer 40k battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "Shooting.h"
#include "Board.h"
#include "Roster.h"
#include <vector>

void ShootingPhase::run(PlayerId player, Board* board) {
/*
    // Get list of models that can shoot
    std::map<Unit*, std::vector<Model*>> shootingUnits;
    auto roster = board->getPlayerRoster(player);
    if (roster != nullptr) {
        for (const auto &up : *roster) {
            if (up->eligibleToShoot()) {
                std::vector<Model *> shootingModels;
                for (const auto &mp : *up) {
                    for (const auto &wp : *mp) {
                        if (wp->isRanged()) {
                            shootingModels.push_back(mp.get());
                            break;
                        }
                    }
                }
                shootingUnits[up.get()] = shootingModels;
            }
        }
    }

    // Get list of potential targets
    std::map<Unit*, std::vector<Model*>> targetUnits;
    auto enemyRoster = board->getPlayerRoster(GetEnemyId(player));
    if (enemyRoster != nullptr) {
        for (const auto &dp : *enemyRoster) {
            for (const auto &up : *dp) {
                std::vector<Model *> targetModels;
                for (const auto &mp : *up) {
                    targetModels.push_back(mp.get());
                }
                targetUnits[up.get()] = targetModels;
            }
        }
    }

    for (auto& up : shootingUnits) {
        auto target = selectTarget(up.first, targetUnits, board);
        if (target) {
            shootAt(up.first, up.second, target);
        }
    }
*/
}

Unit* ShootingPhase::selectTarget(Unit* shooter, std::map<Unit*, std::vector<Model*>> targetUnits, Board* board) const {
    // If shooter is in engagement range, shoot at that target
    for (auto& tp : targetUnits) {
        if (shooter->distanceTo(tp.first) <= ENGAGEMENT_RANGE) {
            return tp.first;
        }
    }

    // Shoot at the unit with the closest visible model that is in range.
    Unit* closestUnit = nullptr;
    double closestDistance = std::numeric_limits<double>::max();
    for (auto& tp : targetUnits) {
        double distance = shooter->distanceTo(tp.first);
        if (board->isVisible(shooter, tp.first) && (distance < closestDistance) /* && (distance < shooter->maxShootingRange())*/) {
            closestDistance = distance;
            closestUnit = tp.first;
        }
    }

    return closestUnit;
}

void ShootingPhase::shootAt(Unit* shooter, std::vector<Model*> shootingModels, Unit* target) const {
    /*
    int totalSlain = 0;
    for (auto &mp : shootingModels) {
        for (auto &wp : *mp) {
            if (wp->isRanged()) {
                auto woundingHits = shooter->attackWithWeapon(wp, target, mp);
                for (auto& wound : woundingHits) {
                    // target makes a save
                    int saveRoll = 0;
                    if (!target->makeSave(wp, shooter, saveRoll)) {
                        // failed save
                        auto numSlain = target->applyDamage(wound, shooter);
                        totalSlain += numSlain;
                    }
                }
            }
        }
    }
    */
}

