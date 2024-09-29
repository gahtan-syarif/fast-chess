# NAME

fastchess - a command-line tool for managing chess games with engines

# SYNOPSIS

fastchess [OPTIONS]

# DESCRIPTION

fastchess is a command-line tool designed to manage and orchestrate chess games between engines. It provides a range of options to configure game settings, engine parameters, concurrency, and output formats.

# OPTIONS

The following options are available:

- -quick cmd=ENGINE1 cmd=ENGINE2 book=BOOK  
    Shortcut for predefined game settings involving two engines, time controls, rounds, and concurrency.

- -event NAME  
    Set the event name for the PGN header.

- -site NAME  
    Set the site name for the PGN header.

- -config [file=NAME] [discard=(true|false)] [outname=NAME] [stats=(true|false)]  
    Load engine configurations to resume games from previous sessions.

    - file - the file name to load the configuration from.
    - discard - discard the loaded configuration after loading. Defaults to false. This lets you specify a filename to save the
      configuration to, while ignoring it for the current session.
    - outname - the auto-generated file name of the config. Default is "config.json".
    - stats - load the stats from the config file. Defaults to true.

- -concurrency N  
    Play N games concurrently, limited by the number of hardware threads. Default value is 1.

- --force-concurrency  
    Ignore the hardware concurrency limit and force the specified concurrency.

- -rounds N  
    Play N rounds of games where each game within the round uses the same opening. Default value is 2.

- -games N  
    Play N games in each round. Default value is 2. Setting this higher than 2 does not provide meaningful results.

- -variant VARIANT  
    Choose between the following game variants:

    - standard - play Standard Chess (default)
    - fischerandom - play Fischer Random Chess

- -repeat  
    Set the number of games to 2. Equivalent to -games 2.

- -recover  
    Enables crash recovery to attempt to recover the engine after a crash and continue the tournament.

- -draw movenumber=NUMBER movecount=COUNT score=SCORE  
    Enables draw adjudication based on number of moves, move count, and score threshold.

    - NUMBER - number of moves before checking for a draw. Default is 0.
    - COUNT - number of consecutive moves below the score threshold. Default is 1.
    - SCORE - score threshold (in centipawns) for a draw. Default is 0.

- -resign movecount=COUNT score=SCORE [twosided=(true|false)]  
    Configures engine resignation based on move count and score threshold.

    - twosided - if true, enables two-sided resignation. Defaults to false.
    - COUNT - number of consecutive moves above the score threshold. Default is 1.
    - SCORE - score threshold (in centipawns) to resign. Default is 0.

- -maxmoves N  
    Enables draw adjudication if the game reaches N moves without a result.

- -openings file=NAME format=(epd|pgn) [order=ORDER] [plies=PLIES] [start=START]  
    Specifies an opening book file and its format for game starting positions.

    - format - file format, either epd or pgn.
    - ORDER - order of openings (random or sequential). Default is sequential.
    - PLIES - number of plies for pgn. Defaults to max available plies.
    - START - starting index of the opening book. Default is 1.

- -output format=FORMAT  
    Choose the output format for game results (cutechess or fastchess). Default is fastchess.

- -pgnout file=NAME notation=(san|lan|uci) [nodes=(true|false)] [seldepth=(true|false)] [nps=(true|false)] [hashfull=(true|false)] [tbhits=(true|false)] [timeleft=(true|false)] [min=(true|false)]  
    Export games in PGN format with specified notations and optional tracking of nodes, seldepth, and others.

    - notation:

        - san - Standard Algebraic Notation (default)
        - lan - Long Algebraic Notation
        - uci - Universal Chess Interface

    - nodes - Track node count. Default is false.
    - seldepth - Track seldepth. Default is false.
    - nps - Track nps. Default is false.
    - hashfull - Track hashfull. Default is false.
    - tbhits - Track tbhits. Default is false.
    - timeleft - Track time left at end of move. Default is false.
    - min - Minimal PGN format. Default is false.

- -epdout file=NAME  
    Export the final position of each game in EPD format.

- -wait N  
    Wait N milliseconds between games. Default is 0.

- -noswap  
    Prevent swapping of colors.

- -reverse  
    Use a tournament schedule with reversed colors.

- -ratinginterval N  
    Set rating interval for the report. Default is 10. Set to 0 to disable.

- -scoreinterval N  
    For cutechess output, set interval for printing score results. Default is 1.

- -autosaveinterval N  
    Automatically saves the tournament state every N games. Default is 20. Set to 0 to disable.

- -sprt elo0=ELO0 elo1=ELO1 alpha=ALPHA beta=BETA model=MODEL  
    Set parameters for the Sequential Probability Ratio Test (SPRT).

    - MODEL:
        - logistic - Uses regular/logistic Elo.
        - bayesian - Uses BayesElo.
        - normalized - Uses nElo (default).

- -srand SEED  
    Specify the seed for opening book randomization.

- -log file=NAME level=LEVEL compress=(true|false) realtime=(true|false)  
    Specify a log file with a specific log level. Set compress to true to gzip the file. Default is false.

    - LEVEL:
        - trace
        - warn (default)
        - info
        - err
        - fatal

- -use-affinity  
    Enable thread affinity for binding engines to CPU cores.

- -report penta=(true|false)  
    Reports pentanomial statistics (for fastchess output). Defaults to true.

- -version  
    Print version number and exit.

- -help  
    Print help message and exit.

- -each OPTIONS  
    Apply specified OPTIONS to both engines.

- -engine OPTIONS  
    Apply specified OPTIONS to the next engine.

    - cmd=COMMAND - Specify engine command.
    - name=NAME - Set engine name (must be unique).
    - args="ARGS" - Pass multiple arguments using args="ARG1 ARG2".
    - tc=TC - Time control in Cute-Chess format (moves/minutes:seconds+increment).
    - timemargin=N - Time margin for exceeding time limit.
    - st=ST - Movetime in seconds.
    - nodes=NODES - Max number of nodes to search.
    - plies=PLIES - Max number of plies (depth) to search.
    - proto=PROTOCOL - Specify the protocol (only supports uci).
    - dir=DIRECTORY - Working directory for the engine.
    - option.name=VALUE - Set engine-specific options.

# EXAMPLES

To start a match between two engines using random openings from book.epd:

```sh
$ fastchess -engine cmd=Engine1.exe name=Engine1 -engine cmd=Engine2.exe \
  name=Engine2 -openings file=book.epd format=epd order=random \
  -each tc=10+0.1 -rounds 200 -repeat -concurrency 4
```

# AUTHORS

fastchess was written by Disservin, Szil, PGG106, and contributors.

# REPORTING BUGS

Report any bugs to <https://github.com/Disservin/fastchess/issues>.

# COPYRIGHT

This software is licensed under the MIT license. See the LICENSE file for details.