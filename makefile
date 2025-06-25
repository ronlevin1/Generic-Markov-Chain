tweets: markov_chain.h markov_chain.c linked_list.h linked_list.c tweets_generator.c
	gcc -Wall -Wextra -Wvla -std=c99 tweets_generator.c markov_chain.c linked_list.c -o tweets_generator

#snakes: markov_chain.h markov_chain.c linked_list.h linked_list.c snakes_and_ladders.c
#	gcc -Wall -Wextra -Wvla -std=c99 snakes_and_ladders.c markov_chain.c linked_list.c -o snakes_and_ladders
	
snake: markov_chain.h markov_chain.c linked_list.h linked_list.c snakes_and_ladders.c
	gcc -Wall -Wextra -Wvla -std=c99 snakes_and_ladders.c markov_chain.c linked_list.c -o snakes_and_ladders
#