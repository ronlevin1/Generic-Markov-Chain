#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY (-1)
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60
#define N3 3
#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20

/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell
{
    int number; // Cell number 1-100
    // ladder_to represents the jump of the ladder
    // in case there is one from this square
    int ladder_to;
    // snake_to represents the jump of the snake
    // in case there is one from this square
    int snake_to;
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

static void *cell_cpy (void *data)
{
  Cell *cur_cell = (Cell *) data;
  Cell *new_cell_ptr = calloc (1, sizeof (Cell));
  if (new_cell_ptr == NULL)
  {
    return NULL;
  }
  memcpy (new_cell_ptr, cur_cell, sizeof (Cell));
  return (void *) new_cell_ptr;
}

static int cell_cmp (void *a, void *b)
{
  Cell *cell1 = (Cell *) a;
  Cell *cell2 = (Cell *) b;
  return ((cell1->number) - (cell2->number));
}

/**
* Check if the given string ends with a period ('.').
* @param data The cell to check.
* @return true if the cell num is 100,
 * false otherwise or if the data is NULL or empty.
*/
static bool is_last_cell (void *data)
{
  // FUNCTION^ USAGE FORMAT: markov_chain->is_last (mk_node->data)
  if (data == NULL)
  {
    return false;
  }
  // if (mk_node->data) is Cell* type
  Cell *cur_cell = (Cell *) data;
  int num = cur_cell->number;
  return (num == BOARD_SIZE);
}

static void cell_print (void *data)
{
  Cell *cur_cell = (Cell *) data;
  int num = cur_cell->number;
  int snake_target = cur_cell->snake_to;
  int ladder_target = cur_cell->ladder_to;

  printf ("[%d]", num);

  if (!is_last_cell (data))
  {
    if (num < ladder_target)
    {
      printf ("-ladder to %d", ladder_target);
    }
    else if (num > snake_target && snake_target != -1)
    {
      printf ("-snake to %d", snake_target);
    }
    printf (" -> ");
  }
}

static void free_cell (void *data)
{
  free (data);
}


/** Error handler **/
static int handle_error (char *error_msg, MarkovChain **database)
{
  printf ("%s", error_msg);
  if (database != NULL)
  {
    free_markov_chain (database);
  }
  return EXIT_FAILURE;
}

static int create_board (Cell *cells[BOARD_SIZE])
{
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    cells[i] = malloc (sizeof (Cell));
    if (cells[i] == NULL)
    {
      for (int j = 0; j < i; j++)
      {
        free (cells[j]);
      }
      handle_error (ALLOCATION_ERROR_MASSAGE, NULL);
      return EXIT_FAILURE;
    }
    *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
  }

  for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
  {
    int from = transitions[i][0];
    int to = transitions[i][1];
    if (from < to)
    {
      cells[from - 1]->ladder_to = to;
    }
    else
    {
      cells[from - 1]->snake_to = to;
    }
  }
  return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database (MarkovChain *markov_chain)
{
  Cell *cells[BOARD_SIZE];
  if (create_board (cells) == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  MarkovNode *from_node = NULL, *to_node = NULL;
  size_t index_to;
  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    add_to_database (markov_chain, cells[i]);
  }

  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    from_node = get_node_from_database (markov_chain, cells[i])->data;

    if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
    {
      index_to = MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
      to_node = get_node_from_database (markov_chain, cells[index_to])
              ->data;
      add_node_to_counter_list (from_node, to_node, markov_chain);
    }
    else
    {
      for (int j = 1; j <= DICE_MAX; j++)
      {
        index_to = ((Cell *) (from_node->data))->number + j - 1;
        if (index_to >= BOARD_SIZE)
        {
          break;
        }
        to_node = get_node_from_database (markov_chain, cells[index_to])
                ->data;
        add_node_to_counter_list (from_node, to_node, markov_chain);
      }
    }
  }
  // free temp arr
  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    free (cells[i]);
  }
  return EXIT_SUCCESS;
}

//static void print_markov_chain (MarkovChain *markov_chain)
//{
//  if (markov_chain == NULL || markov_chain->database == NULL)
//  {
//    printf ("Markov chain is empty.\n");
//    return;
//  }
//
//  int node_number = 1;
//  Node *current_node = markov_chain->database->first;
//  while (current_node != NULL) // && node_number <= 4621)
//  {
//    MarkovNode *current_markov_node = current_node->data;
//    printf ("Node %d: ", node_number++);
//    markov_chain->print_func (current_markov_node->data);
//    printf ("\n");
//
//    for (int i = 0; i < current_markov_node->list_size; i++)
//    {
//      NextNodeCounter *current_freq =
//              &current_markov_node->counter_list[i];
//      printf ("-> ");
//      markov_chain->print_func (current_freq->markov_node->data);
//      printf (" (count: %u)\n", current_freq->counter);
//    }
//    printf ("\tFreq sum: %d\n", get_counters_sum (current_markov_node));
//    current_node = current_node->next;
//  }
//}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences (walks) to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main (int argc, char *argv[])
{
  if (argc != N3)
  {
    printf ("Usage: invalid number of parameters");
    return EXIT_FAILURE;
  }
  /** 0. Initiation */
  unsigned int seed;
  sscanf (argv[1], "%u", &seed);
  srand (seed);

  int num_of_walks;
  sscanf (argv[2], "%d", &num_of_walks);
  // todo all _____
  MarkovChain *markov_chain = alloc_markov_chain ();
  markov_chain->print_func = cell_print;
  markov_chain->comp_func = cell_cmp;
  markov_chain->free_data = free_cell; // or free()?
  markov_chain->copy_func = cell_cpy;
  markov_chain->is_last = is_last_cell;
  if (markov_chain == NULL || fill_database (markov_chain))
  {
    free_markov_chain (&markov_chain);
    return EXIT_FAILURE;
  }
  /** 1. LEARNING PHASE: fill DB & create board - implemented.
           if cur cell has ladder -> go to its end
           else, if cur cell has snake -> go to its end
           else, throw dice and go 1-6 steps ahead.
   */

  /** 2. Generate game: */
  // first cell is always [1]
  MarkovNode *first_cell = markov_chain->database->first->data;
  // get next cell from cur_cell.counter_list, with proportion to its freq.
  // game ends if player got to [100], or after 60 rounds.
  for (int i = 0; i < num_of_walks; ++i)
  {
    printf ("Random Walk %d: ", i + 1);
    generate_random_sequence (markov_chain, first_cell,
                              MAX_GENERATION_LENGTH - 1);
    printf ("\n");
  }
  /** 3. Finish. */
  free_markov_chain (&markov_chain);
  return EXIT_SUCCESS;
}