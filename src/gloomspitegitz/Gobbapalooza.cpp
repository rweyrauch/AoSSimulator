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
    static const int BASESIZE = 32;
    static const int WOUNDS_SCAREMONGER = 4;
    static const int WOUNDS_SHROOMANCER = 4;
    static const int WOUNDS = 3;
    static const int POINTS_PER_UNIT = 200;

    bool Gobbapalooza::s_registered = false;

    Unit *Gobbapalooza::Create(const ParameterList &parameters) {
        auto unit = new Gobbapalooza();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Gobbapalooza::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    nullptr,
                    nullptr,
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
        return POINTS_PER_UNIT;
    }

    Gobbapalooza::Gobbapalooza() :
            GloomspiteGitzBase("Gobbapalooza", 5, WOUNDS, 5, 6, false),
            m_tusksAndFangs(Weapon::Type::Melee, "Boingob's Tusks and Fangs", 1, 4, 4, 3, -1, RAND_D3),
            m_stikka(Weapon::Type::Melee, "Concealed Stikka", 1, 2, 4, 4, -1, 1),
            m_scorpisquigStikka(Weapon::Type::Melee, "Scorpisquig Stikka", 2, 1, 4, 4, 0, RAND_D3),
            m_staff(Weapon::Type::Melee, "Mesmerizing Staff", 1, 1, 4, 3, -1, RAND_D3),
            m_knife(Weapon::Type::Melee, "Moon Staff and Jaggedy Knife", 2, 2, 4, 3, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, GOBBAPALOOZA, SCAREMONGER,
                      BREWGIT, SPIKER, WIZARD, BOGGLEYE, SHROOMANCER};
        m_weapons = {&m_tusksAndFangs, &m_stikka, &m_scorpisquigStikka, &m_staff, &m_knife};
    }

    bool Gobbapalooza::configure() {

        auto scaremonger = new Model(BASESIZE, WOUNDS_SCAREMONGER);
        scaremonger->addMeleeWeapon(&m_tusksAndFangs);
        scaremonger->setName("Scaremonger");
        addModel(scaremonger);

        auto brewgit = new Model(BASESIZE, wounds());
        brewgit->addMeleeWeapon(&m_stikka);
        brewgit->setName("Brewgit");
        addModel(brewgit);

        auto spiker = new Model(BASESIZE, wounds());
        spiker->addMeleeWeapon(&m_scorpisquigStikka);
        spiker->setName("Spiker");
        addModel(spiker);

        auto boggleye = new Model(BASESIZE, wounds());
        boggleye->addMeleeWeapon(&m_staff);
        boggleye->setName("Boggleye");
        addModel(boggleye);

        auto shroomancer = new Model(BASESIZE, WOUNDS_SHROOMANCER);
        shroomancer->addMeleeWeapon(&m_knife);
        shroomancer->setName("Shroomancer");
        addModel(shroomancer);

        m_points = ComputePoints(1);

        return true;
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

    int Gobbapalooza::toSaveModifier(const Weapon *weapon) const {
        auto mod = Unit::toSaveModifier(weapon);

        // Hallucinogenic Fungus Brews
        if (m_battleRound == 1) mod += 2;
        else if (m_battleRound == 2) mod++;

        return mod;
    }

} // namespace GloomspiteGitz
