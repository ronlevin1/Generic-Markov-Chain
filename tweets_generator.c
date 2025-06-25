#include "markov_chain.h"
#include <string.h>

#define MAX_LINE 1000
#define DELIMITERS " \n\r"
#define N4 4
#define N5 5
#define MAX_TWEET 20

static void *my_str_cpy (void *data)
{
  char *str = (char *) data;
  int str_size = (int) strlen (str);
  char *temp = calloc (1, sizeof (char) * (str_size + 1));
  if (temp == NULL)
  {
    return NULL;
  }
  memcpy (temp, str, sizeof (char) * (str_size + 1));
  return (void *) temp;
}

static int my_strcmp (void *a, void *b)
{
  char *str1 = (char *) a;
  char *str2 = (char *) b;
  return strcmp (str1, str2);
}

static void my_print (void *data)
{
  char *str = (char *) data;
  printf (" %s", str);
}

/**
* Check if the given string ends with a period ('.').
* @param ptr The string to check.
* @return true if the string ends with a period,
 * false otherwise or if the string is NULL or empty.
*/
static bool ends_with_period (void *ptr)
{
  char *str = ptr;
  if (str == NULL || strlen (str) == 0)
  {
    return false;
  }

  char last_char = str[strlen (str) - 1];
  return last_char == '.';
}

/**
 * Reads n words from file and fill them in Markov Chain database.
 * @param fp
 * @param words_to_read
 * @param markov_chain
 * @return 0 upon success, 1 otherwise.
 */
static int fill_database (FILE *fp, int words_to_read, MarkovChain
*markov_chain)
{
  int words_read = 0;
  bool flag = true;
  char cur_line[MAX_LINE];

  // 1. Read a whole line
  while (fgets (cur_line, MAX_LINE, fp) && flag)
  {
    // 2. Split line into words
    // 2a. Read first word & add to DB.
    char *word = strtok (cur_line, DELIMITERS);
    Node *word1_node = add_to_database (markov_chain, word);
//    if (word1_node != NULL) // ^ condition if next_word is null, is inside
//    {
    words_read++;
//    }
    while (word != NULL)
    {
      // 2b. Read next word
      char *next_word = strtok (NULL, DELIMITERS);
      if (next_word == NULL)
      {
        break;
      }
      Node *word2_node = add_to_database (markov_chain, next_word);
      if (word2_node != NULL) // ^ condition if next_word is null, is inside
      {
        words_read++;
        // 3. Update word1_node frequencies list (edge cases checked inside)
        if (!add_node_to_counter_list (word1_node->data, word2_node->data,
                                       markov_chain))
        {
          return EXIT_FAILURE;
        }
      }
      // if its null - we finished the line. break inner loop & read new line.
      word = next_word;
      word1_node = word2_node;
      // 4. Check stopping condition
      if (words_read == words_to_read) // || next_word == NULL)
      {
        flag = false;
        break;
      }
    }
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
 *
 * @param argc
 * @param argv
 * @return
 */
int main (int argc, char *argv[])
{
  if (argc < N4 || argc > N5)
  {
    printf ("Usage: invalid number of parameters");
    return EXIT_FAILURE;
  }
  // 0. initiation
  unsigned int seed;
  sscanf (argv[1], "%u", &seed);
  srand (seed);

  int num_of_tweets;
  sscanf (argv[2], "%d", &num_of_tweets);

  FILE *path = fopen (argv[3], "r");
  if (path == NULL)
  {
    printf ("ERROR: file path not valid");
    return EXIT_FAILURE;
  }

  int num_of_words = -1; // condition causing to read all the file.
  if (argc == N5)        // change it if the user gave a value.
  {
    sscanf (argv[4], "%d", &num_of_words);
  }
  // 1. read file & fill database
  MarkovChain *markov_chain = alloc_markov_chain ();
  markov_chain->print_func = my_print;
  markov_chain->comp_func = my_strcmp;
  markov_chain->free_data = free;
  markov_chain->copy_func = my_str_cpy;
  markov_chain->is_last = ends_with_period;

  if (markov_chain == NULL ||
      fill_database (path, num_of_words, markov_chain) == 1)
  {
    free_markov_chain (&markov_chain);
    return EXIT_FAILURE;
  }
  fclose (path);
  // 2. generate tweets
  for (int i = 0; i < num_of_tweets; ++i)
  {
    printf ("Tweet %d:", i + 1);
    generate_random_sequence (markov_chain, NULL, MAX_TWEET - 1);
    printf ("\n");
  }
  free_markov_chain (&markov_chain);
  return EXIT_SUCCESS;
}