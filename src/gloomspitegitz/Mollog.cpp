/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <gloomspitegitz/Mollog.h>
#include <UnitFactory.h>

namespace GloomspiteGitz {
    static const int BASESIZE = 50;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 170;

    bool Mollog::s_registered = false;

    Unit *Mollog::Create(const ParameterList &parameters) {
        auto unit = new Mollog();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Mollog::Init() {
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

            s_registered = UnitFactory::Register("Mollog", factoryMethod);
        }
    }

    int Mollog::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    Mollog::Mollog() :
        GloomspiteGitzBase("Mollog", 6, WOUNDS, 7, 4, false),
            m_jabbertoad(Weapon::Type::Missile, "Jabbertoad", 12, 1, 4, 4, 0, 1),
            m_club(Weapon::Type::Melee, "Puff-fungus Club", 1, 2, 0, 0, 0, 0) {
        m_keywords = {DESTRUCTION, TROGGOTH, GLOOMSPITE_GITZ, DANKHOLD, HERO, TROGGBOSS};
        m_weapons = {&m_jabbertoad, &m_club};

        s_globalBraveryMod.connect(this, &Mollog::reassuringPresence, &m_connection);
    }

    Mollog::~Mollog() {
        m_connection.disconnect();
    }

    bool Mollog::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_jabbertoad);
        model->addMeleeWeapon(&m_club);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    void Mollog::onStartHero(PlayerId player) {
        if (player == owningPlayer()) {
            if (remainingWounds() < WOUNDS && remainingWounds() > 0) {
                // Regeneration - heal D3
                // Troggoth Renewal
                if (Dice::rollD6() >= 4 || (inLightOfTheBadMoon() && (Dice::rollD6() >= 4))) {
                    int woundsHealed = Dice::rollD3();
                    if (inLightOfTheBadMoon())
                        woundsHealed *= 2;
                    for (auto &m : m_models) {
                        m->applyHealing(woundsHealed);
                    }
                }
            }
        }
    }

    int Mollog::reassuringPresence(const Unit *unit) {
        // Reassuring Presence
        if (unit->hasKeyword(GLOOMSPITE_GITZ) && isFriendly(unit) &&
            (distanceTo(unit) <= 12.0f)) {
            return 1;
        }
        return 0;
    }

} // namespace GloomspiteGitz
