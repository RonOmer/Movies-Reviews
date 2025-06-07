# Movie Voting System

## Overview

This project is a C-based console application for managing a movie voting system. Users can add movies, vote for movies, view statistics, and generate recommendations based on voting data. The system reads and writes data to text files, allowing persistent storage of movies and votes.

## Features

- Add new movies to the system.
- Add votes for movies, including country and comments.
- Print all comments and countries for a specific movie.
- List all movies of a certain genre.
- Find movies rated with a specific value from a specific country.
- Count how many different countries rated movies from a specific year.
- Identify the country whose viewers wrote the most comments.
- Generate a file with all movies rated above a certain average value.
- Delete the lowest vote for a movie of a specific genre.
- Data is saved and loaded from `moviesData.txt` and `votingData.txt`.

## Files

- `Qfinal.c` - Main source code file.
- `moviesData.txt` - Stores movie information.
- `votingData.txt` - Stores voting information.
- `Recommendation.txt` - Generated file with recommended movies.

## How to Run

1. Compile the project using a C compiler (C99 or later recommended).
2. Make sure `moviesData.txt` and `votingData.txt` exist in the working directory.
3. Run the executable. Follow the on-screen menu to interact with the system.

## Usage

- Follow the menu options (0-9) to perform actions.
- Input is required as prompted (IDs, names, genres, years, etc.).
- The program updates the data files upon exit.

## Notes

- The program uses dynamic memory allocation; ensure enough system memory is available.
- Input validation is basic; enter data as requested to avoid errors.
- Comments for votes can be left empty by entering `-`.

## License

This project is for educational purposes.