#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <chess.hpp>

#ifdef _WIN64
#    include <engine/process/process_win.hpp>
#else
#    include <engine/process/process_posix.hpp>
#endif

#include <types/engine_config.hpp>

namespace fast_chess::engine {

enum class ScoreType { CP, MATE, ERR };

class UciEngine : protected process::Process {
   public:
    explicit UciEngine(const EngineConfiguration &config) {
        loadConfig(config);
        output_.reserve(100);
    }

    UciEngine(const UciEngine &)            = delete;
    UciEngine(UciEngine &&)                 = delete;
    UciEngine &operator=(const UciEngine &) = delete;
    UciEngine &operator=(UciEngine &&)      = delete;

    ~UciEngine() override { quit(); }

    // Starts the engine, does nothing after the first call.
    void start();

    // Restarts the engine, if necessary and reapplies the options.
    bool refreshUci();

    [[nodiscard]] bool uci();
    [[nodiscard]] bool uciok();
    [[nodiscard]] bool ucinewgame();

    // Sends "isready" to the engine
    [[nodiscard]] bool isready(std::chrono::milliseconds threshold = ping_time_);

    // Sends "position" to the engine and waits for a response.
    [[nodiscard]] bool position(const std::vector<std::string> &moves, const std::string &fen);

    [[nodiscard]] bool go(const TimeControl &our_tc, const TimeControl &enemy_tc, chess::Color stm);

    void quit();

    // Writes the input to the engine. Appends a newline to the input.
    bool writeEngine(const std::string &input);

    // Waits for the engine to output the last_word or until the threshold_ms is reached.
    // May throw if the read fails.
    process::Status readEngine(std::string_view last_word, std::chrono::milliseconds threshold = ping_time_);

    // Logs are not written in realtime to avoid slowing down the engine.
    // This function writes the logs to the logger.
    void writeLog() const;

    void setCpus(const std::vector<int> &cpus) { setAffinity(cpus); }

    // Get the bestmove from the last output.
    [[nodiscard]] std::optional<std::string> bestmove() const;

    [[nodiscard]] std::string lastInfoLine() const;

    // Get the last info from the last output.
    [[nodiscard]] std::vector<std::string> lastInfo() const;

    // Get the last score type from the last output. cp or mate.
    [[nodiscard]] ScoreType lastScoreType() const;

    // Get the last score from the last output. Becareful, mate scores are not converted. So
    // the score might 1, while it's actually mate 1. Always check lastScoreType() first.
    [[nodiscard]] int lastScore() const;

    [[nodiscard]] bool outputIncludesBestmove() const;

    [[nodiscard]] const std::vector<process::Line> &output() const noexcept { return output_; }
    [[nodiscard]] const EngineConfiguration &getConfig() const noexcept { return config_; }
    [[nodiscard]] std::optional<std::string> getUciOptionValue(const std::string &name) const;

    // @TODO: expose this to the user
    static constexpr std::chrono::milliseconds initialize_time = std::chrono::milliseconds(60000);
    static constexpr std::chrono::milliseconds ping_time_      = std::chrono::milliseconds(
#ifdef NDEBUG
        60000
#else
        60000
#endif
    );

   private:
    void loadConfig(const EngineConfiguration &config);
    void sendSetoption(const std::string &name, const std::string &value);

    std::unordered_map<std::string, std::string> uci_options_;
    std::vector<process::Line> output_;
    EngineConfiguration config_;

    // init on first use
    bool initialized_ = false;
};
}  // namespace fast_chess::engine
