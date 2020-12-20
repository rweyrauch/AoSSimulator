/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <wanderers/GladeGuard.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Wanderers {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 360;

    bool GladeGuard::s_registered = false;

    GladeGuard::GladeGuard() :
            Wanderer("Glade Guard", 6, g_wounds, 6, 6, false),
            m_longbow(Weapon::Type::Missile, "Longbow", 20, 1, 4, 4, 0, 1),
            m_longbowLord(Weapon::Type::Missile, "Longbow", 20, 2, 4, 4, 0, 1),
            m_gladeBlade(Weapon::Type::Melee, "Glade Blade", 1, 1, 5, 5, 0, 1) {
        m_keywords = {ORDER, AELF, WANDERER, GLADE_GUARD};
        m_weapons = {&m_longbow, &m_longbowLord, &m_gladeBlade};
    }

    bool GladeGuard::configure(int numModels, bool pennantBearer, bool hornblower) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto lord = new Model(g_basesize, wounds());
        lord->addMissileWeapon(&m_longbowLord);
        lord->addMeleeWeapon(&m_gladeBlade);
        addModel(lord);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_longbow);
            model->addMeleeWeapon(&m_gladeBlade);
            if (pennantBearer) {
                model->setName("Pennant Bearer");
                pennantBearer = false;
            }
            else if (hornblower) {
                model->setName(Model::Hornblower);
                hornblower = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *GladeGuard::Create(const ParameterList &parameters) {
        auto unit = new GladeGuard();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool pennantBearer = GetBoolParam("Pennant Bearer", parameters, false);
        bool hornblower = GetBoolParam("Hornblower", parameters, false);

        bool ok = unit->configure(numModels, pennantBearer, hornblower);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GladeGuard::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GladeGuard::Create,
                    nullptr,
                    nullptr,
                    GladeGuard::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Pennant Bearer"),
                            BoolParameter("Hornblower"),
                    },
                    ORDER,
                    {WANDERER}
            };
            s_registered = UnitFactory::Register("Glade Guard", factoryMethod);
        }
    }

    int GladeGuard::toHitModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = Wanderer::toHitModifier(weapon, target);
        // Peerless Archery
        if (weapon->isMissile() && remainingModels() >= 20) {
            auto closestUnit = Board::Instance()->getNearestUnit(this, owningPlayer());
            if ((closestUnit == nullptr) || (distanceTo(closestUnit) > 3)) {
                modifier += 1;
            }
        }
        return modifier;
    }

    Rerolls GladeGuard::runRerolls() const {
        if (isNamedModelAlive(Model::Hornblower)) {
            return Reroll_Failed;
        }
        return Wanderer::runRerolls();
    }

    int GladeGuard::braveryModifier() const {
        int modifier = Wanderer::braveryModifier();
        if (isNamedModelAlive("Pennant Bearer")) {
            modifier += 1;

            // if (Board::Instance()->unitInCover(this)) { modifier += 1; }
        }
        return modifier;
    }

    void GladeGuard::onStartShooting(PlayerId player) {
        Wanderer::onStartShooting(player);

        m_longbow.setRend(0);
        m_longbowLord.setRend(0);

        if (!m_usedArcaneBodkins) {
            if (m_shootingTarget) {
                // Arcane Bodkins
                m_usedArcaneBodkins = true;

                m_longbow.setRend(-3);
                m_longbowLord.setRend(-3);
            }
        }
    }

    int GladeGuard::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Wanderers
