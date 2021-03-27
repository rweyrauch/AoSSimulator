/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <gloomspitegitz/Gobbapalooza.h>
#include <UnitFactory.h>
#include <Board.h>

namespace GloomspiteGitz {
    static const int g_basesize = 32;
    static const int g_woundsScaremonger = 4;
    static const int g_woundsShroomancer = 4;
    static const int g_wounds = 3;
    static const int g_pointsPerUnit = 200;

    bool Gobbapalooza::s_registered = false;

    Unit *Gobbapalooza::Create(const ParameterList &parameters) {
        auto unit = new Gobbapalooza();

        unit->configure();
        return unit;
    }

    void Gobbapalooza::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };

            s_registered = UnitFactory::Register("Gobbapalooza", factoryMethod);
        }
    }

    int Gobbapalooza::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    Gobbapalooza::Gobbapalooza() :
            GloomspiteGitzBase("Gobbapalooza", 5, g_wounds, 5, 6, false),
            m_tusksAndFangs(Weapon::Type::Melee, "Boingob's Tusks and Fangs", 1, 4, 4, 3, -1, RAND_D3),
            m_stikka(Weapon::Type::Melee, "Concealed Stikka", 1, 2, 4, 4, -1, 1),
            m_scorpisquigStikka(Weapon::Type::Melee, "Scorpisquig Stikka", 2, 1, 4, 4, 0, RAND_D3),
            m_staff(Weapon::Type::Melee, "Mesmerizing Staff", 1, 1, 4, 3, -1, RAND_D3),
            m_knife(Weapon::Type::Melee, "Moon Staff and Jaggedy Knife", 2, 2, 4, 3, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, GOBBAPALOOZA, SCAREMONGER,
                      BREWGIT, SPIKER, WIZARD, BOGGLEYE, SHROOMANCER};
        m_weapons = {&m_tusksAndFangs, &m_stikka, &m_scorpisquigStikka, &m_staff, &m_knife};
    }

    void Gobbapalooza::configure() {

        auto scaremonger = new Model(g_basesize, g_woundsScaremonger);
        scaremonger->addMeleeWeapon(&m_tusksAndFangs);
        scaremonger->setName("Scaremonger");
        addModel(scaremonger);

        auto brewgit = new Model(g_basesize, wounds());
        brewgit->addMeleeWeapon(&m_stikka);
        brewgit->setName("Brewgit");
        addModel(brewgit);

        auto spiker = new Model(g_basesize, wounds());
        spiker->addMeleeWeapon(&m_scorpisquigStikka);
        spiker->setName("Spiker");
        addModel(spiker);

        auto boggleye = new Model(g_basesize, wounds());
        boggleye->addMeleeWeapon(&m_staff);
        boggleye->setName("Boggleye");
        addModel(boggleye);

        auto shroomancer = new Model(g_basesize, g_woundsShroomancer);
        shroomancer->addMeleeWeapon(&m_knife);
        shroomancer->setName("Shroomancer");
        addModel(shroomancer);

        m_points = ComputePoints(1);
    }

    int Gobbapalooza::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);

        // Slippery Git
        if (weapon->isMissile()) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 3.0);
            for (auto unit : units) {
                if (unit->hasKeyword(MOONCLAN) && (distanceTo(unit) <= 3.0)) {
                    mod--;
                    break;
                }
            }
        }
        return mod;
    }

    int Gobbapalooza::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::toSaveModifier(weapon, attacker);

        // Hallucinogenic Fungus Brews
        if (m_battleRound == 1) mod += 2;
        else if (m_battleRound == 2) mod++;

        return mod;
    }

    void Gobbapalooza::onStartHero(PlayerId player) {
        GloomspiteGitzBase::onStartHero(player);

        // Bogeyman
        if (owningPlayer() == player) {
            if (isNamedModelAlive("Scaremonger") && (Dice::RollD6() >= 3)) {
                auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 18.0);
                for (auto unit : units) {
                    if ((unit->remainingModels() > 0) && unit->hasKeyword(MOONCLAN) && unit->hasKeyword(GROT)) {
                        unit->buffReroll(Attribute::Charge_Distance, Rerolls::Failed, {Phase::Hero, m_battleRound+1, owningPlayer()});
                        unit->buffReroll(Attribute::Run_Distance, Rerolls::Failed, {Phase::Hero, m_battleRound+1, owningPlayer()});
                        break;
                    }
                }
            }
        }
        // Loonshine Potion
        if (owningPlayer() == player) {
            if (isNamedModelAlive("Brewgit") && (Dice::RollD6() >= 3)) {
                auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 18.0);
                for (auto unit : units) {
                    if ((unit->remainingModels() > 0) && unit->hasKeyword(MOONCLAN) && unit->hasKeyword(GROT) && unit->hasKeyword(HERO)) {
                        unit->buffReroll(Attribute::To_Hit_Melee, Rerolls::Failed, {Phase::Hero, m_battleRound+1, owningPlayer()});
                        unit->buffReroll(Attribute::To_Hit_Missile, Rerolls::Failed, {Phase::Hero, m_battleRound+1, owningPlayer()});
                        break;
                    }
                }
            }
        }
        // Poison Brewer
        if (owningPlayer() == player) {
            if (isNamedModelAlive("Spiker") && (Dice::RollD6() >= 3)) {
                auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 18.0);
                for (auto unit : units) {
                    if ((unit->remainingModels() > 0) && unit->hasKeyword(MOONCLAN) && unit->hasKeyword(GROT)) {
                        unit->buffReroll(Attribute::To_Wound_Melee, Rerolls::Ones, {Phase::Hero, m_battleRound+1, owningPlayer()});
                        unit->buffReroll(Attribute::To_Wound_Missile, Rerolls::Ones, {Phase::Hero, m_battleRound+1, owningPlayer()});
                        break;
                    }
                }
            }
        }
    }

} // namespace GloomspiteGitz
