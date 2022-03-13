#include "StateCapture.h"
#include <algorithm> //std::sort
#include <vector> //std::vector


StateCapture::StateCapture(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game, std::shared_ptr<Param> param)
        : State(tank, game,
                param) {
}


std::string StateCapture::getType() {
    return "capture";
}


std::vector<Position> intersection(const std::vector<Position> &v1,
                                   std::vector<Position> &v2) {
    std::vector<Position> v3;
    for (auto hex1: v1) {
        for (auto hex2: v2) {
            if (hex1 == hex2) {
                v3.push_back(hex1);
                break;
            }
        }
    }

    return v3;
}

bool sortbysec(const std::pair<Position, int> &a,
               const std::pair<Position, int> &b) {
    return (a.second < b.second);
}

std::vector<std::pair<Position, int>>
calculateShootingVector(std::shared_ptr<Game> game, std::shared_ptr<Tank> tank) {


    std::vector<Position> possible_moves;
    for (auto hex: intersection(tank->getAchievableHexes(game->map), game->map.base)) {
        bool exist = false;
        for (auto tank: game->all_vehicles) {
            if (tank != nullptr && tank->getPosition() == hex) {
                exist = true;
                break;
            }
        }
        if (!exist)
            possible_moves.push_back(hex);
    }

    possible_moves.push_back(tank->getPosition());

    std::vector<std::pair<Position, int>> sorted_hexes;
    sorted_hexes.reserve(possible_moves.size());
    for (const auto &hex: possible_moves) {
        sorted_hexes.emplace_back(hex, game->map.getHex(hex)->danger[0]);
    }

    std::sort(sorted_hexes.begin(), sorted_hexes.end(), sortbysec);

    return sorted_hexes;
}

bool checkPosition(std::vector<std::pair<Position, int>> moves, Position position) {
    for (auto move: moves)
        if (move.first == position && move.second != moves[0].second)
            return true;
        else
            return false;
    return false;
}

std::string StateCapture::calculateAction() {
    auto action_save = game->saveTeam(tank);
    if (!action_save.empty())
        return action_save;

    auto position = tank->getPosition();

    if (std::find(game->map.base.begin(), game->map.base.end(), position) != game->map.base.end()) {
        // Already on base
        return onBaseAction();
    }

    auto shoot = game->selectBestShoot(game->getPossibleShoots(tank, true), tank, true);
    if (!shoot.empty())
        return shootToString(shoot);

    // Not on base
    auto base_positions = game->getSafePositions(tank, game->map.base, false, false);
    if (!base_positions.empty()) {
        auto path = game->smartFindQuickPath(tank->getPosition(), base_positions,
                                             tank);
        if (!path.empty())
            return moveToString(path[1]);
    }

    std::string action = game->getSafeShootAction(tank);
    if (!action.empty())
        return action;

    auto possible_shoots = game->getPossibleShoots(tank, true);
    if (!possible_shoots.empty())
        return shootToString(game->selectBestShoot(possible_shoots, tank, true));

    return "";

}

bool sortbydistance(const Position &a,
                    const Position &b, Position pos) {
    return a.getDistance(pos) < a.getDistance(pos);
}

std::string StateCapture::onBaseAction() {
    if (game->map.getHex(tank->getPosition())->danger[0] > 0) {
        auto shoot = game->selectBestShoot(game->getPossibleShoots(tank, true), tank, true);
        if (!shoot.empty())
            return shootToString(shoot);
        // in danger
//        std::cout << "in danger! ";
        auto safe_position = game->getSafePositions(tank, game->map.base, true, false);
        if (!safe_position.empty()) {
            auto path = game->smartFindQuickPath(tank->getPosition(), safe_position,
                                                 tank);
            if (!path.empty())
                return moveToString(path[1]);
        }

    }
    std::vector<Position> positions;
    for (auto pos: tank->getAchievableHexes(game->map)) {
        if (game->map.getHex(tank->getPosition())->danger[0] < tank->getHealthPoints()) {
            positions.push_back(pos);
        }
    }
    auto spawn_position = tank->getSpawnPosition();
    std::sort(positions.begin(), positions.end(),
              [spawn_position](auto &&PH1, auto &&PH2) {
                  return sortbydistance(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2),
                                        spawn_position);
              });
    if (!positions.empty())
        return moveToString(positions[0]);
    // in safe position OR the only way is to shoot
    auto possible_shoots = game->getPossibleShoots(tank, true);
//    std::cout << possible_shoots.size();
    if (!possible_shoots.empty())
        return shootToString(game->selectBestShoot(possible_shoots, tank, false));

    if (game->map.getHex(tank->getPosition())->danger[0] > 0) {
        //  ЕСЛИ В ОПАСНОСТИ: если мы выигрываем за ход, то остаться | иначе уехать с базы в сейвовую позицию если такая есть
        json capture_state = game->getCaptureState();
        auto player_id = tank->getPlayerId();
        int opponent1 = -1;
        int opponent2 = -1;
        int player = 0;
        int player_capture = 0;
        for (auto it = capture_state.begin(); it != capture_state.end(); ++it) {
            if (std::stoi(it.key()) == player_id) {
                player = it.value()["tanks_on_base"].get<std::int32_t>();
                player_capture = it.value()["capture_points"].get<std::int32_t>();
            } else if (opponent1 == -1) {
                opponent1 = it.value()["tanks_on_base"].get<std::int32_t>();
            } else {
                opponent2 = it.value()["tanks_on_base"].get<std::int32_t>();
            }
        }

        if ((opponent1 == 0 || opponent2 == 0) && player_capture + player >= 5) {
            return "";
        } else {
            auto safe_position = game->getSafePositions(tank, game->map.base, false, true);
            if (!safe_position.empty()) {
                auto path = game->smartFindQuickPath(tank->getPosition(), safe_position,
                                                     tank);
                if (!path.empty())
                    return moveToString(path[1]);
            }
        }


    } else {
        std::vector<Position> positions;
        for (auto pos: tank->getAchievableHexes(game->map)) {
            if (game->map.getHex(tank->getPosition())->danger[0] < tank->getHealthPoints()) {
                positions.push_back(pos);
            }
        }
        auto spawn_position = tank->getSpawnPosition();
        std::sort(positions.begin(), positions.end(),
                  [spawn_position](auto &&PH1, auto &&PH2) {
                      return sortbydistance(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2),
                                            spawn_position);
                  });
        if (!positions.empty())
            return moveToString(positions[0]);
    }

    return "";
}

