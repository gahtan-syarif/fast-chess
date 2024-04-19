#pragma once

#include <vector>

#include <affinity/affinity_manager.hpp>
#include <engines/uci_engine.hpp>
#include <matchmaking/book/opening_book.hpp>
#include <matchmaking/output/output.hpp>
#include <matchmaking/result.hpp>
#include <types/tournament_options.hpp>
#include <util/cache.hpp>
#include <util/file_writer.hpp>
#include <util/logger/logger.hpp>
#include <util/threadpool.hpp>

namespace fast_chess {

namespace atomic {
extern std::atomic_bool stop;
}  // namespace atomic

class BaseTournament {
   public:
    BaseTournament(const options::Tournament &config,
                   const std::vector<EngineConfiguration> &engine_configs);

    virtual ~BaseTournament() = default;

    virtual void start();

    /// @brief forces the tournament to stop
    virtual void stop();

    [[nodiscard]] stats_map getResults() noexcept { return result_.getResults(); }
    void setResults(const stats_map &results) noexcept { 
       result_.setResults(results);

       uint64_t total_wins = 0;
       uint64_t total_losses = 0;
       uint64_t total_draws = 0;
       
       // Iterate over the outer map
       for (const auto &pair1 : result_.getResults()) {
           const auto &inner_map = pair1.second;
           // Iterate over the inner map
           for (const auto &pair2 : inner_map) {
               const Stats &stats = pair2.second;
               total_wins += stats.wins;
               total_losses += stats.losses;
               total_draws += stats.draws;
           }
       }
       match_count_ = total_wins + total_losses + total_draws;
       total_       = total_wins + total_losses + total_draws;
       initial_id_  = match_count_;
    }

    void setGameConfig(const options::Tournament &tournament_config) noexcept {
        tournament_options_ = tournament_config;
    }

   protected:
    /// @brief number of games played
    std::atomic<uint64_t> match_count_;
    /// @brief number of games to be played
    std::atomic<uint64_t> total_;
    uint64_t initial_id_;

    /// @brief creates the matches
    virtual void create() = 0;

    using start_callback    = std::function<void()>;
    using finished_callback = std::function<void(const Stats &stats, const std::string &reason)>;

    /// @brief play one game and write it to the pgn file
    /// @param configs
    /// @param start
    /// @param finish
    /// @param opening
    /// @param game_id
    void playGame(const std::pair<EngineConfiguration, EngineConfiguration> &configs,
                  start_callback start, finished_callback finish, const Opening &opening,
                  std::size_t game_id);

    std::unique_ptr<IOutput> output_;
    std::unique_ptr<affinity::AffinityManager> cores_;
    std::unique_ptr<FileWriter> file_writer_;

    OpeningBook book_;
    options::Tournament tournament_options_;
    std::vector<EngineConfiguration> engine_configs_;

    CachePool<UciEngine, std::string> engine_cache_ = CachePool<UciEngine, std::string>();
    Result result_                                  = Result();
    ThreadPool pool_                                = ThreadPool(1);

   private:
    int getMaxAffinity(const std::vector<EngineConfiguration> &configs) const noexcept;
};

}  // namespace fast_chess
