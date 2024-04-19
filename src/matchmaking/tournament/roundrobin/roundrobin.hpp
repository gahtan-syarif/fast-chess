#pragma once

#include <affinity/affinity_manager.hpp>
#include <matchmaking/match/match.hpp>
#include <matchmaking/result.hpp>
#include <matchmaking/sprt/sprt.hpp>
#include <pgn/pgn_reader.hpp>
#include <types/stats.hpp>
#include <types/tournament_options.hpp>
#include <util/cache.hpp>
#include <util/file_writer.hpp>
#include <util/rand.hpp>
#include <util/threadpool.hpp>

#include <matchmaking/tournament/base/tournament.hpp>

namespace fast_chess {

namespace atomic {
extern std::atomic_bool stop;
}  // namespace atomic

class RoundRobin : public BaseTournament {
   public:
    explicit RoundRobin(const options::Tournament &tournament_config,
                        const std::vector<EngineConfiguration> &engine_configs);

    /// @brief starts the round robin
    void start() override;

   protected:
    /// @brief creates the matches
    void create() override;

   private:
    /// @brief update the current running sprt. SPRT Config has to be valid.
    /// @param engine_configs
    void updateSprtStatus(const std::vector<EngineConfiguration> &engine_configs);

    SPRT sprt_ = SPRT();
};
}  // namespace fast_chess
