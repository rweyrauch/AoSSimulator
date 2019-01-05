/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHAMMERSIM_UNIT_H
#define WARHAMMERSIM_UNIT_H

#include <memory>
#include <vector>
#include <string>
#include "Weapon.h"
#include "Model.h"

class Unit
{
public:
    Unit() = default;
    Unit(const std::string& name, int move, int wounds, int bravery, int save, bool fly);
    virtual ~Unit() = default;

    const std::string& name() const { return m_name; }
    int wounds() const { return m_wounds; }
    int move() const { return m_move; }
    int bravery() const { return m_bravery; }
    int save() const { return m_save; }
    bool fly() const { return m_fly; }

    void addModel(const Model& model);

    virtual void beginTurn();

    virtual void hero();
    virtual void movement(bool run = false);
    virtual void shooting(int numAttackingModels, Unit& unit);
    virtual void charge();
    virtual int combat(int numAttackingModels, Unit& unit);
    virtual int battleshock(int modifier = 0);
    virtual int takeDamage(int numWoundingHits, const Weapon& weapon);

    virtual int applyDamage(int totalDamage);

    int remainingModels() const { return (int)m_models.size(); }

private:
    std::string m_name;
    int m_move = 0;
    int m_wounds = 0;
    int m_bravery = 0;
    int m_save = 0;

    bool m_fly = false;
    bool m_runAndShoot = false;
    bool m_runAndCharge = false;
    bool m_ignoreRend = false;

    std::vector<Model> m_models;

    int m_modelsSlain = 0;
    int m_ran = false;
    int m_charged = false;
};


#endif //WARHAMMERSIM_UNIT_H
