cmake_minimum_required(VERSION 3.24)
project(ex3b_ron.levin1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(tweets
        linked_list.c
        linked_list.h
        markov_chain.c
        markov_chain.h
        tweets_generator.c)

add_executable(snakes
        linked_list.c
        linked_list.h
        markov_chain.c
        markov_chain.h
        snakes_and_ladders.c)