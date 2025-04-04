#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#define SIZE 1000000

void enqueue(struct queue *q, struct game_state state) 
{
    insert_at_tail(&q->data, serialize(state)); 
}

struct game_state dequeue(struct queue *q) 
{
    size_t val = remove_from_head(&q->data); 
    return deserialize(val); 
}

int number_of_moves(struct game_state start) 
{
    struct queue q; 
    q.data.head = NULL; 

    uint64_t *used = calloc(SIZE, sizeof(uint64_t)); //allocating my space for an array, wnt it to be initialized as 0; 
    if(used == NULL)
    {
        return -1;
    }

    uint64_t startpt = serialize(start); 
    used[startpt % SIZE] = startpt; //used to change startpt to depth inside array 
    enqueue(&q, start); 

        while(q.data.head != NULL)
        {
            struct game_state current = dequeue(&q); 
            int end = 1; 
            int tile_r = 1;

            for(int i = 0; i < 4 && (end != 0); i++)
            {
                for(int j = 0; j < 4 && (end != 0); j++)
                {
                    int tile = current.tiles[i][j];

                    if (i == 3 && j == 3) 
                    {
                        if (tile != 0)
                        { 
                            end = 0;
                        }
                        
                    }
                    else 
                    {
                        if (tile != tile_r++) 
                        {
                            end = 0;
                        }
                    }
                }
            }
            
            if (end) 
            {
            free_list(q.data);
            free(used);
            return current.num_steps;
            }
            for(int i = 0; i < 4; i++)
            {
                struct game_state next_pos = current;
        
                if(i ==0)
                {
                    move_up(&next_pos);
                }
        
                if(i == 1)
                {
                    move_down(&next_pos);
                }
        
                if(i ==2)
                {
                    move_left(&next_pos);
                }
        
                if(i == 3)
                {
                    move_right(&next_pos);
                }
        
                if(next_pos.num_steps != current.num_steps)
                {
                    uint64_t s_state = serialize(next_pos);
                    size_t location = s_state % SIZE; 
                    int num_seen = 0; 
                    int target = 0; 
        
                    for(int j = 0; j < SIZE && !(num_seen || target); j++)
                    {
                        size_t tester = (location + j) % SIZE; 
        
                        if(used[tester] == 0)
                        {
                            used[tester] = s_state;
                            target = 1; 
                        }
        
                        else if(used[tester] == s_state)
                        {
                            num_seen = 1; 
                        }
                    }
        
                    if(!num_seen)
                    {
                        enqueue(&q, next_pos);
                    }
                } 
            }
        }

    // if (end) 
    // {
    //     free_list(q.data);
    //     free(visited);
    //     return current.num_steps;
    // }

    // for(int i = 0; i < 4; i++)
    // {
    //     struct game_state next_pos = current;

    //     if(i ==0)
    //     {
    //         move_up(&next_pos);
    //     }

    //     if(i == 1)
    //     {
    //         move_down(&next_pos);
    //     }

    //     if(i ==2)
    //     {
    //         move_left(&next_pos);
    //     }

    //     if(i == 3)
    //     {
    //         move_right(&next_pos);
    //     }

    //     if(next_pos.num_steps != current.num_steps)
    //     {
    //         uint64_t s_state = serialize(next_pos);
    //         size_t location = s_state % SIZE; 
    //         int num_seen = 0; 
    //         int target = 0; 

    //         for(int j = 0; j < SIZE && !(num_seen || target); j++)
    //         {
    //             size_t tester = (location + j) % SIZE; 

    //             if(used[tester] == 0)
    //             {
    //                 used[tester] = s_state;
    //                 target = 1; 
    //             }

    //             else if(used[tester] == s_state)
    //             {
    //                 num_seen = 1; 
    //             }
    //         }

    //         if(!num_seen)
    //         {
    //             enqueue(&q, next_pos);
    //         }
    //     } 
    // }

 free_list(q.data); 
 free(used);
 return -1; 
}
