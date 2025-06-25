#include "markov_chain.h"
#include <string.h>

//#ifdef OH_TESTS
//#include "/cs/usr/oryan.hassidim/ex3b/ex3b-oryan.hassidim/
// oh_tests/oh_tests_helpers.h"
//#endif // OH_TESTS
// terminal command:
//python3
// /cs/usr/oryan.hassidim/ex3b/ex3b-oryan.hassidim/oh_tests/test_ex3.b_s.py

int get_random_number (int max_number)
{
  return rand () % max_number;
}

MarkovChain *alloc_markov_chain ()
{
  MarkovChain *new_chain = calloc (1, sizeof (MarkovChain));
  if (new_chain == NULL)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    free (new_chain);
    return NULL;
  }
  new_chain->database = NULL;
  new_chain->print_func = NULL;
  new_chain->comp_func = NULL;
  new_chain->free_data = NULL;
  new_chain->copy_func = NULL;
  new_chain->free_data = NULL;
  return new_chain;
}

int get_counters_sum (MarkovNode *markov_node)
{
  int sum = 0;
  if (markov_node != NULL)
  {
    for (int i = 0; i < markov_node->list_size; ++i)
    {
      sum += (int) markov_node->counter_list[i].counter;
    }
  }
  return sum;
}

/**
 * Initialize a new MarkovNode with the given data.
 * @param data_ptr The data to be stored in the MarkovNode.
 * @return A pointer to the newly initialized MarkovNode if successful,
 * or NULL if memory allocation fails.
 */
MarkovNode *init_markov_node (copy_func_p copy_func, void *data_ptr)
{
  if (data_ptr == NULL)
  {
    return NULL;
  }
  MarkovNode *new_mk_node = calloc (1, sizeof (MarkovNode));
  if (new_mk_node == NULL)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    free (new_mk_node);
    return NULL;
  }
  // initialize node fields
  // todo
  new_mk_node->data = copy_func (data_ptr); // copy data
  if (new_mk_node->data == NULL)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  new_mk_node->list_size = 0;
  new_mk_node->counter_list = NULL;
  return new_mk_node;
}

/**
 * Add a new MarkovNode with the given data to the MarkovChain's database.
 * If successful, returns a pointer to the newly added MarkovNode;
 * otherwise, returns NULL.
 * @param markov_chain Pointer to the MarkovChain to which the MarkovNode
 * will be added.
 * @param data_ptr Pointer to the data to be stored in the new MarkovNode.
 * @return Pointer to new MarkovNode on success, or NULL on failure.
 */
Node *add_markov_node (MarkovChain *markov_chain, void *data_ptr)
{
  MarkovNode *new_mk_node = init_markov_node (markov_chain->copy_func,
                                              data_ptr);
  if (new_mk_node == NULL)
  {
    free (new_mk_node);
    printf (ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  if (add ((markov_chain)->database, new_mk_node) == EXIT_FAILURE)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  return (markov_chain)->database->last;
}

/**
 * Allocate memory for a new LinkedList and associate it with the given
 * MarkovChain.
 * @param markov_chain The MarkovChain to associate the new LinkedList with.
 * @return A pointer to the newly allocated LinkedList if successful,
 * or NULL if memory allocation fails.
 */
LinkedList *alloc_linked_list (MarkovChain *markov_chain)
{
  LinkedList *new_list = calloc (1, sizeof (LinkedList));
  if (new_list == NULL)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    free (new_list);
    return NULL;
  }
  markov_chain->database = new_list;
  return new_list;
}

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain ptr to MarkovChain struct.
 * @return MarkovNode ptr to a random state in chain.
 */
MarkovNode *get_first_random_node_helper (MarkovChain *markov_chain)
{
  int k = get_random_number (markov_chain->database->size);
  Node *node = markov_chain->database->first;
  for (int i = 0; i < k; ++i)
  {
    node = node->next;
  }
  return node->data;
}

MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  MarkovNode *mk_node = get_first_random_node_helper (markov_chain);
  while (markov_chain->is_last (mk_node->data))
  { // get another one
    mk_node = get_first_random_node_helper (markov_chain);
  }
  return mk_node;
}

/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)
{
  // 1. Get frequencies sum & a random number 'k' in [0,count_sum)
  int count_sum = get_counters_sum (state_struct_ptr);
  int k = get_random_number (count_sum);
  NextNodeCounter *ctr_lst = state_struct_ptr->counter_list;

  int i = -1;
  while (k >= 0)
  {
    i++;
    k -= (int) ctr_lst[i].counter;
  }
  return ctr_lst[i].markov_node;
}

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with,
 *                  if NULL- choose a random markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_random_sequence (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
  if (first_node == NULL)
  {
    first_node = get_first_random_node (markov_chain);
  }
  markov_chain->print_func (first_node->data); // print
  MarkovNode *prev_mk_node = first_node;
  int i = 0;
  while (i < max_length)
  {
    MarkovNode *new_mk_node = get_next_random_node (prev_mk_node);
    markov_chain->print_func (new_mk_node->data); // print
    if (markov_chain->is_last (new_mk_node->data))
    {
      break;
    }
    prev_mk_node = new_mk_node;
    i++;
  }
//  printf ("\n"); // todo: change?
}

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_markov_chain (MarkovChain **markov_chain)
{
  if (markov_chain == NULL || *markov_chain == NULL
      || (*markov_chain)->database == NULL)
  {
    return;
  }
  else
  {
    Node *node = (*markov_chain)->database->first;
    while (node != NULL)
    { // todo: use (*markov_chain)->free_data() instead?
      MarkovNode *mk_node = node->data;
      free (mk_node->counter_list);  // free counter_list layer
      mk_node->counter_list = NULL;
      (*markov_chain)->free_data (mk_node->data);  // free data
      mk_node->data = NULL;
      free (mk_node); // free markov nodes layer
      mk_node = NULL;
      Node *prev_node = node; // iterate nodes
      node = node->next;
      prev_node->next = NULL;
      free (prev_node); // free Nodes layer
      prev_node = NULL;
    }
    free (node);
    node = NULL;
    (*markov_chain)->database->first = NULL;
    (*markov_chain)->database->last = NULL;
    free ((*markov_chain)->database);  // free Linked List
    (*markov_chain)->database = NULL;
    free (*markov_chain); // free Chain.
    *markov_chain = NULL;
  }
}

/**
 * Add the second markov_node to the counter list of the first markov_node.
 * If already in list, update it's counter value.
 * @param first_node
 * @param second_node
 * @param markov_chain
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool add_node_to_counter_list (MarkovNode *first_node, MarkovNode
*second_node, MarkovChain *markov_chain)
{ // todo:             V identify last node below
  if (markov_chain->is_last (first_node->data) || second_node == NULL
      || second_node->data == NULL)
  { // dont touch.
    return true;
  }
  /** Check if word2 is already in list */
  NextNodeCounter *node1_ctr_lst = first_node->counter_list;
  int lst1_size = first_node->list_size;
  for (int i = 0; i < lst1_size; i++)
  {
    /** CASE 1: word2 in list - update it */
    if (markov_chain->comp_func (node1_ctr_lst->markov_node->data,
                                 second_node->data) == 0)
    { //                       ^ = cur word in counters_list.
      (node1_ctr_lst->counter)++;
      return true;
    }
    node1_ctr_lst++; // assuming it will be in an array
  }
  /** CASE 2: word2 not in list - insert it
  * Allocate more memory:
  * realloc works like malloc if NULL pointer is sent to it (if lst1_size=0).
  */
  NextNodeCounter *temp = realloc (first_node->counter_list,
                                   (lst1_size + 1)
                                   * sizeof (NextNodeCounter));
  if (temp == NULL)
  {
    return false;
  }
  first_node->counter_list = temp;
  (first_node->list_size)++;
  // Put 'second_node' in the end of ctr_list & set counter to 1
  ((first_node->counter_list) + lst1_size)->markov_node = second_node;
  ((first_node->counter_list) + lst1_size)->counter = 1;
  return true;
}

/**
* Check if data_ptr is in database. If so, return the markov_node
 * wrapping it in the markov_chain. otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node *get_node_from_database (MarkovChain *markov_chain, void *data_ptr)
{
  if (markov_chain == NULL || markov_chain->database == NULL)
    // || markov_chain->database->first == NULL)
  {
    return NULL;
  }

  Node *cur_node = markov_chain->database->first;
  if (markov_chain->database->size == 1 &&
      markov_chain->comp_func (cur_node->data->data, data_ptr) == 0)
  {
    return cur_node;
  }

  while (cur_node != NULL)
  {
    if (markov_chain->comp_func (cur_node->data->data, data_ptr) == 0)
    {
      return cur_node;
    }
    cur_node = cur_node->next;
  }
  return NULL;
}

/**
* If data_ptr in markov_chain, return it's node. Otherwise, create new
 * node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return node wrapping given data_ptr in given chain's database
 */
Node *add_to_database (MarkovChain *markov_chain, void *data_ptr)
{
  // 0. Edge cases
  if (data_ptr == NULL) // || strcmp (data_ptr, "") == 0)
  {
    return NULL;
  }

  // CASE 1: new empty database
  if (markov_chain->database == NULL)
  {
    if (!alloc_linked_list (markov_chain))
    {
      return NULL;
    }
    return add_markov_node (markov_chain, data_ptr);
  }

  // CASE 2: find node in database
  Node *node = get_node_from_database (markov_chain, data_ptr);
  if (node != NULL)
  {
    return node;
  }
  else // data not in database
  {
    return add_markov_node (markov_chain, data_ptr);
  }
}
