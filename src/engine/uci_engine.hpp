#pragma once

#include <string>
#include <vector>

#include <chess.hpp>

#ifdef _WIN64
#include <engine/process/process_win.hpp>
#else
#include <engine/process/process_posix.hpp>
#endif

#include <types/engine_config.hpp>

namespace fast_chess::engine {

enum class ScoreType { CP, MATE, ERR };

class UciEngine : protected process::Process {
   public:
    explicit UciEngine(const EngineConfiguration &config) {
        loadConfig(config);
        start();
        output_.reserve(100);
    }

    ~UciEngine() override { quit(); }

    void refreshUci();

    void uci();
    void quit();
    [[nodiscard]] bool uciok();
    [[nodiscard]] bool ucinewgame();

    // Sends "isready" to the engine and waits for a response.
    [[nodiscard]] bool isResponsive(std::chrono::milliseconds threshold = ping_time_);

    // Waits for the engine to output the last_word or until the threshold_ms is reached.
    // May throw if the read fails.
    process::Status readEngine(std::string_view last_word,
                               std::chrono::milliseconds threshold = ping_time_);

    void writeLog() const;

    [[nodiscard]] std::string lastInfoLine() const;

    // Writes the input to the engine. May throw if the write fails.
    void writeEngine(const std::string &input);

    void setCpus(const std::vector<int> &cpus) { setAffinity(cpus); }

    // Get the bestmove from the last output.
    [[nodiscard]] std::string bestmove() const;

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

    // @TODO: expose this to the user
    static constexpr std::chrono::milliseconds initialize_time = std::chrono::milliseconds(1000);
    static constexpr std::chrono::milliseconds ping_time_      = std::chrono::milliseconds(
#ifdef NDEBUG
        1000
#else
        1000
#endif
    );

   private:
    void start();

    void loadConfig(const EngineConfiguration &config);
    void sendSetoption(const std::string &name, const std::string &value);

    EngineConfiguration config_;

    std::vector<process::Line> output_;
};
}  // namespace fast_chess::engine
