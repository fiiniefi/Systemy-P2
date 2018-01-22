#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <semaphore.h>
//#include <thread>

int hunters_numb, cooks_numb, meat_numb, meal_numb;
sem_t meat;
sem_t meal;
void *hunter_routine(void *);
void *cook_routine(void *);
void consume(int &);


//int cooks_job, hunters_job; //debug

void *hunter_routine(void *)
{
//hunters_job++; //debug
    srand(time(NULL));
    int throw_hunter = std::rand() % 6;
    int throw_prey = std::rand() % 6;
    sem_wait(&meat);
    if (throw_hunter > throw_prey)
    {
        meat_numb++;
        sem_post(&meat);
    }
    sem_wait(&meal);
    consume(hunters_numb);
    sem_post(&meal);
    usleep(100);
    pthread_exit(NULL);
}


void *cook_routine(void *)
{
//cooks_job++; //debug
    sem_wait(&meat);
    if (meat_numb > 0)
    {
        meat_numb--;
        sem_post(&meat);
        sem_wait(&meal);
        meal_numb += (std::rand() % 6);
    }
    consume(cooks_numb);
    sem_post(&meal);
    usleep(100);
    pthread_exit(NULL);
}

void consume(int &villagers_numb)
{
    if (meal_numb > 0)
        meal_numb--;
    else if (villagers_numb > 0)
        villagers_numb--;
}


int main(int argc, char *argv[])
{
    sem_init(&meat, 0, 1);
    sem_init(&meal, 0, 1);
    if (argc < 5)
    {
        std::cout << "Program " << argv[0] << " needs 4 arguments: hunters_numb, cooks_numb, meat_numb, meal_numb" << std::endl;
        return 0;
    }
  
    hunters_numb = atoi(argv[1]), cooks_numb = atoi(argv[2]);
    meat_numb = atoi(argv[3]), meal_numb = atoi(argv[4]);
    
    pthread_t hunters[hunters_numb];
    pthread_t cooks[cooks_numb];

    for (int i = 1 ; i <= 365 ; i ++)
    {
        for (int j = 0 ; j < cooks_numb ; j++)
        {
            int flag = pthread_create(&cooks[j], NULL, cook_routine, NULL);
            if (flag)
                std::cout<< "Error: Cook thread cannot be created" << std::endl;
        }
 
        for (int j = 0 ; j < hunters_numb ; j++)
        {
            int flag = pthread_create(&hunters[j], NULL, hunter_routine, NULL);
            if (flag)
                std::cout << "Error: Hunter thread cannot be created" << std::endl;
        }

        for (int j = 0 ; j < cooks_numb ; j++)
        {
            pthread_join(cooks[j], NULL);
        }
 
        for (int j = 0 ; j < hunters_numb ; j++)
        {
            pthread_join(hunters[j], NULL);
        }
        std::cout << "Number of hunters after " << i << " day(s): " << hunters_numb << std::endl;
        std::cout << "Number of cooks after " << i << " day(s): " << cooks_numb << std::endl;
        std::cout << "Number of meat after " << i << " day(s): " << meat_numb << std::endl;
        std::cout << "Number of meal after " << i << " day(s): " << meal_numb << std::endl;
//std::cout<<"Cooks job: "<<cooks_job<<std::endl; //debug
//std::cout<<"Hunters job: "<<hunters_job<<std::endl; //debug
    }
}
