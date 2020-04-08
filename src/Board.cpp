/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <vector>
#include <algorithm>

#ifdef HAVE_CAIRO
#include <cairommconfig.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#endif

#include <Board.h>
#include <Roster.h>
#include <sstream>
#include <cfloat>

Board *Board::s_pInstance = nullptr;

void Board::addObjective(Objective *objective) {
    m_objectives.push_back(objective);
}

void Board::moveObjective(int id, double x, double y) {
    auto matchId = [id](const Objective *obj) -> bool {
        return (obj->m_id == id);
    };

    auto obj = std::find_if(m_objectives.begin(), m_objectives.end(), matchId);
    if (obj != m_objectives.end()) {
        (*obj)->m_pos.x = x;
        (*obj)->m_pos.y = y;
    }
}

void Board::addFeature(TerrainFeature *feature) {
    m_features.push_back(feature);
}

void Board::addRosters(Roster *pRedRoster, Roster *pBlueRoster) {
    m_rosters[(int) PlayerId::Red] = pRedRoster;
    m_rosters[(int) PlayerId::Blue] = pBlueRoster;
}

Board *Board::Instance() {
    if (s_pInstance == nullptr) {
        s_pInstance = new Board();
    }
    return s_pInstance;
}

void Board::render(const std::string &filename) const {
#ifdef HAVE_CAIRO
    // use cairomm to create a raster image of the current board state
    int imageW = (int) (m_width * 10.0); // tenth's of inches
    int imageH = (int) (m_depth * 10.0);

    Cairo::RefPtr<Cairo::ImageSurface> surface =
        Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, imageW, imageH);

    Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

    Cairo::RefPtr<Cairo::ToyFontFace> font =
        Cairo::ToyFontFace::create("Bitstream Charter", Cairo::FONT_SLANT_ITALIC, Cairo::FONT_WEIGHT_BOLD);

    cr->save(); // save the state of the context
    cr->set_source_rgb(25 / 255.0, 119 / 255.0, 48 / 255.0);
    cr->paint(); // fill image with the color
    cr->restore(); // color is back to black now

    cr->save();
    // draw a border around the image
    cr->set_line_width(2.0); // make the line wider
    cr->rectangle(0.0, 0.0, surface->get_width(), surface->get_height());
    cr->stroke();
    cr->restore();

    // Draw red team units.
    cr->save();
    cr->set_line_width(1.0);

    Roster *red = m_rosters[0];
    for (auto ip = red->unitBegin(); ip != red->unitEnd(); ++ip)
    {
        const Unit *unit = *ip;
        auto baseSize = unit->basesizeInches();
        auto radiusInches = baseSize * 0.5;

        cr->set_source_rgb(1.0, 0.0, 0.0);
        for (auto mip = unit->modelBegin(); mip != unit->modelEnd(); ++mip)
        {
            if (mip->slain() || mip->fled())
            {
                continue;
            }
            auto pos = mip->position();
            cr->arc(pos.x * 10.0, pos.y * 10.0, radiusInches * 10.0, 0.0, 2.0 * M_PI);
            cr->fill();
        }

        auto upos = unit->position();
        cr->set_source_rgb(0.5, 0.0, 0.0);
        cr->arc(upos.x * 10.0, upos.y * 10.0, radiusInches * 10.0, 0.0, 2.0 * M_PI);
        cr->fill();
    }
    cr->restore();

    // Draw blue team units.
    cr->save();
    cr->set_line_width(1.0);

    Roster *blue = m_rosters[1];
    for (auto ip = blue->unitBegin(); ip != blue->unitEnd(); ++ip)
    {
        const Unit *unit = *ip;
        auto baseSize = unit->basesizeInches();
        auto radiusInches = baseSize * 0.5;

        cr->set_source_rgb(0.0, 0.0, 1.0);
        for (auto mip = unit->modelBegin(); mip != unit->modelEnd(); ++mip)
        {
            if (mip->slain() || mip->fled())
            {
                continue;
            }

            auto pos = mip->position();
            cr->arc(pos.x * 10.0, pos.y * 10.0, radiusInches * 10.0, 0.0, 2.0 * M_PI);
            cr->fill();
        }

        auto upos = unit->position();
        cr->set_source_rgb(0.0, 0.0, 0.5);
        cr->arc(upos.x * 10.0, upos.y * 10.0, radiusInches * 10.0, 0.0, 2.0 * M_PI);
        cr->fill();
    }
    cr->restore();

    for (auto ip = red->unitBegin(); ip != red->unitEnd(); ++ip)
    {
        const Unit *unit = *ip;
        auto baseSize = unit->basesizeInches();
        auto radiusInches = baseSize * 0.5;

        // label with the number of remaining models
        cr->save();
        cr->move_to((unit->position().x - 2.0 * radiusInches) * 10, (unit->position().y - radiusInches) * 10);
        cr->set_source_rgb(1, 1, 1);
        cr->set_font_face(font);
        cr->set_font_size(12.0);
        std::stringstream text;
        text << unit->remainingModels();
        cr->show_text(text.str());
        cr->restore();
    }

    for (auto ip = blue->unitBegin(); ip != blue->unitEnd(); ++ip)
    {
        const Unit *unit = *ip;
        auto baseSize = unit->basesizeInches();
        auto radiusInches = baseSize * 0.5;

        // label with the number of remaining models
        cr->save();
        cr->move_to((unit->position().x + radiusInches) * 10, (unit->position().y - radiusInches) * 10);
        cr->set_source_rgb(1, 1, 1);
        cr->set_font_face(font);
        cr->set_font_size(12.0);
        std::stringstream text;
        text << unit->remainingModels();
        cr->show_text(text.str());
        cr->restore();
    }

    surface->write_to_png(filename);
#endif
}

std::vector<Unit *> Board::getUnitsWithin(const Unit *unit, PlayerId which, double distance) {
    std::vector<Unit *> units;
    if (which == PlayerId::None) {
        if (m_rosters[0] != nullptr) {
            for (auto ip = m_rosters[0]->unitBegin(); ip != m_rosters[0]->unitEnd(); ++ip) {
                if (*ip == unit) {
                    continue;
                }
                double dist = unit->distanceTo(*ip);
                if (dist <= distance) {
                    units.push_back(*ip);
                }
            }
        }
        if (m_rosters[1] != nullptr) {
            for (auto ip = m_rosters[1]->unitBegin(); ip != m_rosters[1]->unitEnd(); ++ip) {
                if (*ip == unit) {
                    continue;
                }
                double dist = unit->distanceTo(*ip);
                if (dist <= distance) {
                    units.push_back(*ip);
                }
            }
        }
    } else {
        if (m_rosters[(int) which] != nullptr) {
            for (auto ip = m_rosters[(int) which]->unitBegin(); ip != m_rosters[(int) which]->unitEnd(); ++ip) {
                if (*ip == unit) {
                    continue;
                }
                double dist = unit->distanceTo(*ip);
                if (dist <= distance) {
                    units.push_back(*ip);
                }
            }
        }
    }
    return units;
}

Unit *Board::getNearestUnit(const Unit *unit, PlayerId fromPlayer) {
    if (unit == nullptr) {
        return nullptr;
    }
    Unit *nearestUnit = nullptr;

    if (fromPlayer == PlayerId::None) {
        Unit *nearestRedUnit = m_rosters[0]->nearestUnit(unit);
        Unit *nearestBlueUnit = m_rosters[1]->nearestUnit(unit);
        auto dtoRed = unit->distanceTo(nearestRedUnit);
        auto dtoBlue = unit->distanceTo(nearestBlueUnit);
        if (dtoRed < dtoBlue) {
            nearestUnit = nearestRedUnit;
        } else {
            nearestUnit = nearestBlueUnit;
        }
    } else {
        nearestUnit = m_rosters[(int) fromPlayer]->nearestUnit(unit);
    }
    return nearestUnit;
}

Unit *Board::getUnitWithKeyword(const Unit *unit, PlayerId fromPlayer, Keyword keyword, double distance) {
    auto units = getUnitsWithin(unit, fromPlayer, distance);

    for (auto ip : units) {
        if (ip->hasKeyword(keyword)) {
            return ip;
        }
    }

    return nullptr;
}

bool Board::unbindAttempt(Unit *caster, int castingRoll) {
    int targetId = (int) GetEnemyId(caster->owningPlayer());
    if (m_rosters[targetId]) {
        for (auto ip = m_rosters[targetId]->unitBegin(); ip != m_rosters[targetId]->unitEnd(); ++ip) {
            if ((*ip)->unbind(caster, castingRoll)) {
                return true;
            }
        }
    }

    return false;
}

std::vector<Unit *> Board::getUnitWithin(Board::Quadrant quadrant, PlayerId fromPlayer) {
    const Math::Plane northSouth(Math::Vector3(0.0, 1.0, 0.0), m_depth / 2.0);
    const Math::Plane eastWest(Math::Vector3(1.0, 0.0, 0.0), m_width / 2.0);

    std::vector<Unit *> units;
    if (fromPlayer == PlayerId::None) {
        if (m_rosters[0] != nullptr) {
            for (auto ip = m_rosters[0]->unitBegin(); ip != m_rosters[0]->unitEnd(); ++ip) {
                if (inQuadrant(quadrant, northSouth, eastWest, (*ip)->position())) {
                    units.push_back(*ip);
                }
            }
        }
        if (m_rosters[1] != nullptr) {
            for (auto ip = m_rosters[1]->unitBegin(); ip != m_rosters[1]->unitEnd(); ++ip) {
                if (inQuadrant(quadrant, northSouth, eastWest, (*ip)->position())) {
                    units.push_back(*ip);
                }
            }
        }
    } else {
        if (m_rosters[(int) fromPlayer] != nullptr) {
            for (auto ip = m_rosters[(int) fromPlayer]->unitBegin();
                 ip != m_rosters[(int) fromPlayer]->unitEnd(); ++ip) {
                if (inQuadrant(quadrant, northSouth, eastWest, (*ip)->position())) {
                    units.push_back(*ip);
                }
            }
        }
    }
    return units;
}

std::vector<Unit *> Board::getAllUnits(PlayerId fromPlayer) {
    std::vector<Unit *> units;
    if (fromPlayer == PlayerId::None) {
        if (m_rosters[0] != nullptr) {
            for (auto ip = m_rosters[0]->unitBegin(); ip != m_rosters[0]->unitEnd(); ++ip) {
                units.push_back(*ip);
            }
        }
        if (m_rosters[1] != nullptr) {
            for (auto ip = m_rosters[1]->unitBegin(); ip != m_rosters[1]->unitEnd(); ++ip) {
                units.push_back(*ip);
            }
        }
    } else {
        if (m_rosters[(int) fromPlayer] != nullptr) {
            for (auto ip = m_rosters[(int) fromPlayer]->unitBegin();
                 ip != m_rosters[(int) fromPlayer]->unitEnd(); ++ip) {
                units.push_back(*ip);
            }
        }
    }
    return units;
}

bool Board::inQuadrant(Board::Quadrant quadrant, const Math::Plane &northSouth, const Math::Plane &eastWest,
                       const Math::Point3 &position) const {
    switch (quadrant) {
        case Northwest:
            if (northSouth.contains(position) && !eastWest.contains(position))
                return true;
            break;
        case Northeast:
            if (northSouth.contains(position) && eastWest.contains(position))
                return true;
            break;
        case Southeast:
            if (!northSouth.contains(position) && eastWest.contains(position))
                return true;
            break;
        case Southwest:
            if (!northSouth.contains(position) && !eastWest.contains(position))
                return true;
            break;
    }
    return false;
}

bool Board::isUnitWithin(Quadrant quadrant, const Unit *unit) const {
    const Math::Plane northSouth(Math::Vector3(0.0, 1.0, 0.0), m_depth / 2.0);
    const Math::Plane eastWest(Math::Vector3(1.0, 0.0, 0.0), m_width / 2.0);

    if (unit) {
        return inQuadrant(quadrant, northSouth, eastWest, unit->position());
    }
    return false;
}

std::vector<Unit *> Board::getUnitsWithin(const Math::Point3 &point, PlayerId which, double distance) {
    std::vector<Unit *> units;
    if (which == PlayerId::None) {
        if (m_rosters[0] != nullptr) {
            for (auto ip = m_rosters[0]->unitBegin(); ip != m_rosters[0]->unitEnd(); ++ip) {
                double dist = point.distance((*ip)->position());
                if (dist <= distance) {
                    units.push_back(*ip);
                }
            }
        }
        if (m_rosters[1] != nullptr) {
            for (auto ip = m_rosters[1]->unitBegin(); ip != m_rosters[1]->unitEnd(); ++ip) {
                double dist = point.distance((*ip)->position());
                if (dist <= distance) {
                    units.push_back(*ip);
                }
            }
        }
    } else {
        if (m_rosters[(int) which] != nullptr) {
            for (auto ip = m_rosters[(int) which]->unitBegin(); ip != m_rosters[(int) which]->unitEnd(); ++ip) {
                double dist = point.distance((*ip)->position());
                if (dist <= distance) {
                    units.push_back(*ip);
                }
            }
        }
    }
    return units;
}

bool Board::isVisible(const Unit *from, const Unit *target) {
    if ((from == nullptr) || (target == nullptr)) {
        return false;
    }

    // TODO: check line of sight between from->target

    return true;
}

bool Board::castRay(const Math::Ray &ray, Math::RayHit &result) const {
    result.m_t = 0.0;
    return false;
}

bool Board::castRay(const Math::Ray2 &ray, Math::RayHit &result) const {
    result.m_t = 0.0;
    return false;
}

bool Board::moveModel(Model &model, const Math::Point3 &toPoint) const {
    return false;
}

const Objective *Board::getNearestObjective(const Unit *unit) {
    if (unit == nullptr || m_objectives.empty()) {
        return nullptr;
    }
    Objective *nearestObjective = m_objectives.front();
    double minDistance = DBL_MAX;
    for (auto o : m_objectives) {
        double dist = unit->distanceTo(o->m_pos);
        if (dist < minDistance) {
            minDistance = dist;
            nearestObjective = o;
        }
    }
    return nearestObjective;
}
