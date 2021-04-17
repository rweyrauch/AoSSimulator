/*
 * Warhammer 40k battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once
#include <map>
#include <vector>
#include "Weapon.h"
#include "Phase.h"

class Unit;
class Model;
class Board;

class ShootingPhase : public Phase {
public:

    void run(PlayerId player, Board* board) override;

private:

    Unit* selectTarget(Unit* shooter, std::map<Unit*, std::vector<Model*>> targetUnits, Board* board) const;
    void shootAt(Unit* shooter, std::vector<Model*> shootingModels, Unit* target) const;

};

