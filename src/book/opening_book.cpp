#include <book/opening_book.hpp>

#include <fstream>
#include <optional>
#include <string>

#include <util/safe_getline.hpp>

namespace fast_chess::book {

OpeningBook::OpeningBook(const options::Tournament& tournament) {
    start_ = tournament.opening.start;
    games_ = tournament.games;
    order_ = tournament.opening.order;
    plies_ = tournament.opening.plies;
    setup(tournament.opening.file, tournament.opening.format);
}

void OpeningBook::setup(const std::string& file, FormatType type) {
    if (file.empty()) {
        return;
    }

    std::visit([](auto&& arg) { arg.clear(); }, book_);

    if (type == FormatType::PGN) {
        book_ = pgn::PgnReader(file, plies_).getOpenings();

        if (std::get<pgn_book>(book_).empty()) {
            throw std::runtime_error("No openings found in PGN file: " + file);
        }
    } else if (type == FormatType::EPD) {
        std::ifstream in(file, std::ios::binary | std::ios::ate);
        if (!in) {
            throw std::runtime_error("Error opening EPD file: " + file);
        }

        std::streamsize size = in.tellg();
        in.seekg(0, std::ios::beg);

        file_data_ = std::make_unique<char[]>(size);

        in.read(file_data_.get(), size);

        const char* data  = file_data_.get();
        const char* end   = data;
        const char* start = data;

        while (*end) {
            if (*end == '\n' || *end == '\0') {
                if (end != start) {
                    std::get<epd_book>(book_).emplace_back(std::string_view(start, end - start));
                }
                start = end + 1;
            }
            end++;
        }

        // Handle the last line if it doesn't end with a newline
        if (end != start) {
            std::get<epd_book>(book_).emplace_back(std::string_view(start, end - start));
        }

        std::get<epd_book>(book_).shrink_to_fit();
    }

    if (order_ == OrderType::RANDOM && type != FormatType::NONE) shuffle();
}

[[nodiscard]] std::optional<std::size_t> OpeningBook::fetchId() noexcept {
    static uint64_t opening_index = 0;

    // - 1 because start starts at 1 in the opening options
    const auto idx       = start_ - 1 + opening_index++ + matchcount_ / games_;
    const auto book_size = std::holds_alternative<epd_book>(book_)
                               ? std::get<epd_book>(book_).size()
                               : std::get<pgn_book>(book_).size();

    if (book_size == 0) {
        return std::nullopt;
    }

    return idx % book_size;
}

}  // namespace fast_chess::book
