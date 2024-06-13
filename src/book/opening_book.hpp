#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <pgn/pgn_reader.hpp>
#include <types/enums.hpp>
#include <types/tournament_options.hpp>
#include <util/rand.hpp>

namespace fast_chess::book {

class OpeningBook {
   public:
    OpeningBook() = default;
    explicit OpeningBook(const options::Tournament& tournament);

    // Fisher-Yates / Knuth shuffle
    void shuffle() {
        const auto shuffle = [this](auto& vec) {
            if (order_ == OrderType::RANDOM) {
                for (std::size_t i = 0; i + 2 <= vec.size(); i++) {
                    auto rand     = util::random::mersenne_rand();
                    std::size_t j = i + (rand % (vec.size() - i));
                    std::swap(vec[i], vec[j]);
                }
            }
        };

        std::visit(shuffle, book_);
    }

    // Rotates the book vector based on the starting offset
    void rotate() {
        const auto rotate = [this](auto& vec) {
            std::rotate(vec.begin(), vec.begin() + (offset_ % vec.size()), vec.end());
        };

        std::visit(rotate, book_);
    }

   // Gets rid of unused openings within the book vector to reduce memory usage
   void truncate() {
        const auto truncate = [this](auto& vec) {
            if (vec.size() > static_cast<std::size_t>(rounds_)) {
                vec.erase(vec.begin() + rounds_, vec.end());
                vec.shrink_to_fit();
            }
        };

        std::visit(truncate, book_);
    }

    // Allocate new buffer
    void trim_and_compact() {
        const auto compact = [this](auto& vec) {
            // Calculate total size needed for file_data_
            std::size_t total_size = 0;
            for (const auto& sv : vec) {
                total_size += sv.size() + 1; // +1 for '\n' or '\0'
            }

            // Allocate new memory for file_data_
            std::unique_ptr<char[]> new_file_data = std::make_unique<char[]>(total_size);
            char* ptr = new_file_data.get();

            // Copy data from vec to new_file_data
            for (const auto& sv : vec) {
                std::copy(sv.begin(), sv.end(), ptr);
                *ptr++ = '\n'; // Add newline separator
            }

            // Replace old file_data_ with the new one
            file_data_ = std::move(new_file_data);
        };

        std::visit(compact, book_);
    }

    [[nodiscard]] std::optional<std::size_t> fetchId() noexcept;

    void setInternalOffset(std::size_t offset) noexcept { matchcount_ = offset; }

    pgn::Opening operator[](std::optional<std::size_t> idx) const noexcept {
        if (!idx.has_value()) return {chess::constants::STARTPOS, {}};

        if (std::holds_alternative<epd_book>(book_)) {
            const auto fen = std::get<epd_book>(book_)[*idx];
            return {std::string(fen), {}, chess::Board(fen).sideToMove()};
        }

        return std::get<pgn_book>(book_)[*idx];
    }

   private:
    void setup(const std::string& file, FormatType type);

    using epd_book = std::vector<std::string_view>;
    using pgn_book = std::vector<pgn::Opening>;

    std::size_t start_      = 0;
    std::size_t matchcount_ = 0;
    std::size_t offset_     = 0;
    int rounds_;
    int games_;
    int plies_;
    OrderType order_;
    std::variant<epd_book, pgn_book> book_;

    std::unique_ptr<char[]> file_data_;
};

}  // namespace fast_chess::book
