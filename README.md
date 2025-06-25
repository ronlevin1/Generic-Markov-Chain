# Markov Chain Implementation in C

**Short description:**  
This project provides a generic implementation of a Markov chain in C and demonstrates its application through two different programs: a "Snakes and Ladders" game simulator and a text-based tweet generator.

## Features
- 🔧 **Generic Markov Chain:** A flexible and reusable Markov chain data structure.
- 🐍 **Snakes and Ladders:** A simulation of the classic board game, where player movements are determined by the Markov chain.
- 🐦 **Tweet Generator:** A program that learns from a given text corpus to generate new tweets, showcasing the text generation capabilities of Markov chains.
- ⚙️ **Technologies:** The project is built using C and can be compiled using a Makefile.

## Getting Started
Clone the repo and run:

```bash
git clone https://github.com/ronlevin1/Generic-Markov-Chain
cd project-name
make all
```

### Running the programs

**Snakes and Ladders**

To run the game simulator, use the following command:
```bash
./snakes_and_ladders <seed> <number_of_walks>
```
- `<seed>`: An unsigned integer for the random number generator.
- `<number_of_walks>`: The number of game simulations to run.

**Tweet Generator**

To generate tweets, use the following command:
```bash
./tweets_generator <seed> <number_of_tweets> <path_to_corpus> [words_to_read]
```
- `<seed>`: An unsigned integer for the random number generator.
- `<number_of_tweets>`: The number of tweets to generate.
- `<path_to_corpus>`: The file path to the text file to learn from.
- `[words_to_read]`: (Optional) The maximum number of words to read from the corpus. If not provided, the entire file will be read.

## Demo / Screenshot

![Terminal Screenshot](demo_run)
